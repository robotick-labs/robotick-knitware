import os

# Set the folder path here
folder_path = "C:/Users/paul.connor/OneDrive - Keywords Studios Ltd/Pictures/New folder"

# Loop through all files in the folder
for filename in os.listdir(folder_path):
    file_path = os.path.join(folder_path, filename)
    if os.path.isfile(file_path):
        name_without_ext = os.path.splitext(filename)[0]
        new_filename = name_without_ext + ".png"
        new_file_path = os.path.join(folder_path, new_filename)
        os.rename(file_path, new_file_path)
        print(f"Renamed: {filename} -> {new_filename}")
