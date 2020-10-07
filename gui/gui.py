from tkinter import *
from tkinter import filedialog
from tkinter import ttk
import subprocess

def generate(*args):
    if(folder_in.get() == folder_out.get()):
        status.set("Error: folders cannot be the same")
        return
    status.set("Started")
    root.update_idletasks()
    command = StringVar()
    command.set("./bin/doc_gen")
    subprocess.run([command.get(), folder_in.get(), folder_out.get()])
    status.set("Complete")

def search_folder_in(*args):
    folder_in.set(filedialog.askdirectory())

def search_folder_out(*args):
    folder_out.set(filedialog.askdirectory())

root = Tk()
root.title = ("Documentation Generator")

mainframe = ttk.Frame(root, padding="6 6 6 6")
mainframe.grid(column = 0, row = 0, sticky = (N, W, E, S))
root.columnconfigure(0, weight = 1)
root.rowconfigure(0, weight = 1)

folder_in = StringVar()
folder_out = StringVar()
status = StringVar()
status.set("Not Started")

ttk.Label(mainframe, text = "Input Folder:").grid(column = 1, row = 1, sticky = (W, E))
folder_in_entry = ttk.Entry(mainframe, textvariable = folder_in)
folder_in_entry.grid(column = 2, row = 1, sticky = (W, E))
ttk.Button(mainframe, text = "Search", command = search_folder_in).grid(column = 3, row = 1, sticky = (W, E))

ttk.Label(mainframe, text = "Output Folder:").grid(column = 1, row = 2, sticky = (W, E))
folder_out_entry = ttk.Entry(mainframe, textvariable = folder_out)
folder_out_entry.grid(column = 2, row = 2, sticky = (W, E))
ttk.Button(mainframe, text = "Search", command = search_folder_out).grid(column = 3, row = 2, sticky = (W, E))

ttk.Button(mainframe, text = "Generate", command = generate).grid(column = 2, row = 3, sticky = (W, E))

ttk.Label(mainframe, text = "Status:").grid(column = 1, row = 4, sticky = (W, E))
ttk.Label(mainframe, textvariable = status).grid(column = 2, row = 4, sticky = (W, E))

for child in mainframe.winfo_children():
    child.grid_configure(padx = 4, pady = 4)

folder_in_entry.focus()
root.bind('<Return>', generate)

root.mainloop()
