import os



# Am I in the correct directory?

os.chdir(r"C:\Users\Gebruiker\Documents\Courses & Electives\Design for emotion\fotos into the wild\fotos")
print(os.getcwd())

# Print all the current file names
i=0

# >>> a = ['Mary', 'had', 'a', 'little', 'lamb']
# >>> for i in range(len(a)): //for instances in a list of 5 (list is also created)
# ...     print(i, a[i])
files = os.listdir()
for i in range(len(files)):
    # print(file)

    file_name, file_ext = os.path.splitext(files[i])

    # if file_ext=='.pdf':
    file_name='Meulendijks_IntoTheWild_'+str(i)+file_ext
    os.rename(files[i], file_name)
