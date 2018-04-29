import random

events = 100
rooms = 5
features = 5
students = 80

stud_even_trash = 8
room_features_trash = 60
event_featues_trash = 10

print("{0} {1} {2} {3}".format(events, rooms, features, students))  # basic arguments
print("90\n40\n30\n30\n50")  # EDIT room sizes


for i in range(students * events):
    x = random.randint(0, 100)
    if (x < stud_even_trash):
        print("1")
    else:
        print("0")

for i in range(rooms * features):
    x = random.randint(0, 100)
    if (x < room_features_trash):
        print("1")
    else:
        print("0")

for i in range(events * features):
    x = random.randint(0, 100)
    if (x < event_featues_trash):
        print("1")
    else:
        print("0")
