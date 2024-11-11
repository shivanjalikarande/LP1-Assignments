import tkinter as tk
from tkinter import messagebox

"""Initializing main window"""
root = tk.Tk()
root.title("PICT Hostel Management System")
root.geometry("900x700")
root.configure(bg="white")

def show_frame(frame):
    frame.tkraise()

"""Colors"""
bg_color = "#f0f0f0"
navbar_color = "blue"
text_color="#ffffff"
button_color="#99ccff"
card_color = "#cce7ff"
card_text = "#333333"
footer_color = "#333333"

"""creating different frames"""
main_frame = tk.Frame(root,bg=bg_color)
login_frame = tk.Frame(root,bg=bg_color)
signup_frame = tk.Frame(root,bg=bg_color)
guidelines_frame = tk.Frame(root,bg=bg_color)

for frame in(main_frame,login_frame,signup_frame,guidelines_frame):
    frame.grid(row=1,column=0,stick="nsew")

root.grid_rowconfigure(1,weight=1)  
root.grid_columnconfigure(0,weight=1)


""" A. main page content"""
pict_label = tk.Label(main_frame,text="PICT",font=("Arial",24,"bold"),bg=bg_color)
pict_label.pack(pady=20)
title_page = tk.Label(main_frame,text="PUNE INSTITUTE OF COMPUTER TECHNOLOGY",font=("Arial",18),bg=bg_color)
title_page.pack(pady=10)


""" A.a) navbar """
navbar_frame = tk.Frame(root,bg=navbar_color,height=50)
navbar_frame.grid(row=0,column=0,stick="ew")

navbar_buttons = ["Home", "Guidelines"]
for btn in navbar_buttons:
    btn = tk.Button(navbar_frame,text=btn,bg=button_color,fg=text_color,font=("Arial",12),command=lambda b=btn: navbar_links(b))
    btn.pack(side="left",padx=15, pady=15)

navbar_buttons = ["Signup","Login"]
for btn in navbar_buttons:
    btn = tk.Button(navbar_frame,text=btn,fg=text_color,bg=button_color,font=("Arial",12),command=lambda b=btn: navbar_links(b))
    btn.pack(side="right",padx=15,pady=15)

def navbar_links(button_name):
    if button_name == "Home":
        show_frame(main_frame)
    elif button_name == "Login":
        show_frame(login_frame)
    elif button_name == "Signup":
        show_frame(signup_frame)
    elif button_name == "Guidelines":
        show_frame(guidelines_frame)

""" A.b) cards on main page"""
def show_cards(frame,side,title,desc):
    card_frame = tk.Frame(frame,bg=card_color,padx=20,pady=20)
    if side == "left":
        card_frame.pack(anchor="w",pady=20,padx=20)
    elif side == "right":
        card_frame.pack(anchor="e",pady=20,padx=20)

    cards_title = tk.Label(card_frame,text=title,font=("Arial",14))
    cards_title.pack(anchor="w")
    cards_desc = tk.Label(card_frame,text=desc,font=("Arial",12))
    cards_desc.pack(anchor="w")

features = [
    ("Hostel Facilities", "We provide excellent hostel facilities including mess, laundry, and 24/7 security services."),
    ("Student Attendance", "Track student attendance effectively with our digital attendance management system."),
    ("Complaint Resolution", "A streamlined system for students to raise complaints and get quick resolutions."),
    ("Health and Safety", "Health and safety measures are our priority with regular health checkups and emergency services.")
]
    
cards_frame = tk.Frame(main_frame,bg=bg_color)
cards_frame.pack(pady=20,fill="x")
for i, (title,desc) in enumerate(features):
    side = "left" if i%2 == 0 else "right"
    show_cards(cards_frame,side,title,desc)


""" A. c) Footer """
footer_frame = tk.Frame(root,bg=footer_color)
footer_frame.grid(row=2,column=0,stick="ew")

address_label = tk.Label(footer_frame,text="Address:\nPICT, Near Bharati Vidypeeth,\nDhankwadi, Pune - 411043",
                        fg=text_color, bg=footer_color, font=("Arial", 10))
address_label.pack(side="left",padx=20,pady=10)

hours_label = tk.Label(footer_frame, text="Office Hours:\nMon-Fri: 9am-5pm\nSat: 10am-2pm\nSun: Closed",
                       fg=text_color, bg=footer_color, font=("Arial", 10))
hours_label.pack(side="left", padx=20, pady=20)

contact_label = tk.Label(footer_frame, text="Contact:\nEmail: info@pict.edu\nPhone: +91-1234567890",
                         fg=text_color, bg=footer_color, font=("Arial", 10))
contact_label.pack(side="right", padx=20, pady=20)

# Social Media icons (placeholder)
social_media_frame = tk.Frame(footer_frame, bg=footer_color)
social_media_frame.pack(side="right", padx=10, pady=10)

social_icons = ["F", "T", "I", "G"]  # Facebook, Twitter, Instagram, Google (placeholders)
for icon in social_icons:
    icon_label = tk.Label(social_media_frame, text=icon, width=2, bg="#666666", fg=text_color, font=("Arial", 12))
    icon_label.pack(side="left", padx=5)


""" B. Guidelines Page """
guidelines_title = tk.Label(guidelines_frame, text="Guidelines", font=("Arial", 24), bg=bg_color)
guidelines_title.pack(pady=20)

guidelines_text = """
1. Students must adhere to the hostel timings strictly.
2. Visitors are allowed only during specified hours.
3. No loud music or noise after 10 PM.
4. Maintain cleanliness in rooms and common areas.
5. Any damage to hostel property must be reported immediately.
6. Ragging in any form is strictly prohibited.
"""
guidelines_label = tk.Label(guidelines_frame, text=guidelines_text, font=("Arial", 12), bg=bg_color, justify="left")
guidelines_label.pack(pady=10)


""" C. LOGIN PAGE """
login_title = tk.Label(login_frame,text="LOGIN",bg=bg_color,font=("Arial",24,"bold"))
login_title.pack(pady=15)

username_label = tk.Label(login_frame,text="Username",bg=bg_color,font=("Arial",14))
username_label.pack(pady=5)
username_entry = tk.Entry(login_frame,width=30)
username_entry.pack(pady=5)

password_label = tk.Label(login_frame,text="Password",bg=bg_color,font=("Arial",14))
password_label.pack(pady=5)
password_entry = tk.Entry(login_frame,width=30)
password_entry.pack(pady=5)

login_button = tk.Button(login_frame,text="Login",bg=button_color,fg=text_color,font=("Arial",14),command= lambda: messagebox.showinfo("Login","Login successfull"))
login_button.pack(pady=20)

""" D. SIGNUP PAGE"""
signup_title = tk.Label(signup_frame,text="SIGNUP",bg=bg_color,font=("Arial",24,"bold"))
signup_title.pack(pady=15)

fields = ["Full Name", "Class", "Division", "Username", "Password"]
entries = {}

for field in fields:
    label = tk.Label(signup_frame, text=f"{field}:", font=("Arial", 14), bg=bg_color)
    label.pack(pady=5)
    entry = tk.Entry(signup_frame, width=30)
    if field == "Password":
        entry.config(show="*")
    entry.pack(pady=5)
    entries[field] = entry

register_button = tk.Button(signup_frame, text="Register", bg=button_color, fg=text_color, font=("Arial", 14),
                            command=lambda: messagebox.showinfo("Sign-Up", "Registration Successful"))
register_button.pack(pady=20)

"""to run the program"""
show_frame(main_frame)
root.mainloop()
