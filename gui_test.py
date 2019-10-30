from guizero import App, Text, TextBox, PushButton
app = App(title="gui_messing about")
welcome_message = Text(app, text="", size=random(40), font="Comic Sans MS", color="blue")

my_name = TextBox(app)

def say_my_name():
    welcome_message.value = my_name.value

update_text = PushButton(app, command=say_my_name,text="enter")

app.display()
