#!/usr/bin/env python
import cgi, os
# import cgitb; cgitb.enable()

form = cgi.FieldStorage()

print('FORM:')
print(form.getfirst("user", "...").upper())    # This way it's safe.
for item in form.getlist("item"):
    print(item)
print(form)
print('END')

# fileitem = form.getvalue('filename')

# if fileitem.file:
#     fn = os.path.basename(fileitem.file)
#     # print("what is " + fn);
#     # open('/Users/jinhyeok/Desktop/42seoul/git_webserver/cpp_webserver/04_file_read/' + fn, 'wb').write(fileitem.file.read())

#     message = 'The file "' + fn + '" was uploaded successfully'

# else:
#     message = 'No file was uploaded'

# print("""
# <!DOCTYPE html>
# <html lang="en">
# <head>
# 	<meta charset="UTF-8">
# 	<meta http-equiv="X-UA-Compatible" content="IE=edge">
# 	<meta name="viewport" content="width=device-width, initial-scale=1.0">
# 	<title>Little Webserver</title>
# </head>

# <body>
#    <p>%s</p>
#    <form action='upload.py' method='post' enctype='multipart/form-data'>
#    <input type='file' name='filename'><br>
#    <input type='submit' value='Upload'><br>
#    </form>
# </body>
# </html>""" % (message,))
