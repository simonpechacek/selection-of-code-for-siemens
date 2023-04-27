import PySimpleGUI as sg
import shutil
import json
import os
from threading import Thread, active_count
sg.theme("DarkGrey13")


def load_templates():
    try:
        with open("templates/templates.json", "r") as f:
            temp = json.load(f)
            f.close()
            return temp

    except IOError:
        sg.PopupError()


def update_list():
    win["template"].update(list(load_templates().keys()))


def get_template_name(template):
    try:
        with open("templates/templates.json", "r") as f:
            temp = json.load(f)
            f.close()
            name = temp[template]

            extension = name.split(".")[1]
            return name, extension
    except (IOError, KeyError) as e:
        sg.popup_error()


def create_project(target, template, name):
    template_name, extension = get_template_name(template)
    source_path = f"templates/{template_name}"

    target_path = target + f"/{name}.{extension}"

    shutil.copyfile(source_path, target_path)


def validate_input(values):
    if len(values["folder_path"]) == 0:
        return False
    if len(values["name"]) == 0:
        return False
    if len(values["template"]) == 0:
        return False

    return True


def save_template(name, file_path):
    extension = file_path.split(".")[1]
    target = f"templates/template_{name}.{extension}"

    shutil.copyfile(file_path, target)

    with open("templates/templates.json", "r") as f:
        templates = json.load(f)

        templates.update({name: f"template_{name}.{extension}"})
        f.close()

    with open("templates/templates.json", "w") as f:
        f.write(json.dumps(templates))
        f.close()


def add_template():
    temp_layout = [
        [sg.Text("Tamplate name")],
        [sg.Input(key="temp_name")],
        [sg.Text("Template file")],
        [sg.Input(key="file_path"), sg.FileBrowse()],
        [sg.Button("Add"), sg.Button("Cancel")]
    ]
    temp_win = sg.Window("Add template", layout=temp_layout)

    event, values = temp_win.read()

    if event == "Add":
        save_template(values["temp_name"], values["file_path"])

    update_list()
    temp_win.close()
    del temp_win


def edit_template(template):
    os.system(f"idle -e templates/{templates[template]}")


templates = load_templates()
sg.set_options()

layout = [
    [sg.Text("Choose project template")],
    [sg.Listbox(list(templates.keys()), size=(25, min(5, len(templates) + 1)), key="template"),sg.VerticalSeparator(),
     sg.Button("Add template"), sg.Button("Edit template")],
    [sg.Button("Delete template")],
    [sg.Text("Choose project Folder")],
    [sg.Input(key="folder_path"), sg.FolderBrowse()],
    [sg.Text("Enter project name (extension will be copied from chosen template)")],
    [sg.Input(key="name")],
    [sg.Button("Create"), sg.Button("Exit")]
]

win = sg.Window("Project Creator", layout=layout)

while True:
    event, values = win.read()
    if event in (sg.WIN_CLOSED, "Exit"):
        if active_count() > 1:
            sg.Popup("Save and close the template!")
        else:
            break

    if event == "Create":
        if validate_input(values):
            create_project(values["folder_path"], values["template"][0], values["name"])

            if sg.popup_yes_no("Its all done! Quit?") == "Yes":
                break
            else:
                win["folder_path"].update("")
                win["name"].update("")
        else:
            sg.popup_ok("You forgot something")
    if event == "Add template":
        add_template()
    if event == "Edit template":
        Thread(target=edit_template, args=(values["template"][0], )).start()

win.close()
del win
