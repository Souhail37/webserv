#!/usr/local/bin/python3

import cgi
import os

form = cgi.FieldStorage()

if 'file' in form:
    print ("----WAch Dkhlti l fichier ")
    # Get the uploaded file
    file_item = form['file']
    upload_dir = os.environ['UPLOAD_DIR']

    # Check if the file was uploaded
    if file_item.filename:
        # Open a file for writing
        file = open(upload_dir + file_item.filename, 'wb')

        # Write the file to the server
        file.write(file_item.file.read())
        file.close()
        message = 'File Uploaded Successfully'
    else:
        message = "Error uploading file";
else:
    message = 'No file was uploaded'

print("Content-type: text/html\n")
print("<html>")
print("<body>")
print("<p>%s</p>" % message)
print("</body>")
print("</html>")