#!/usr/bin/env python

import cgi

form = cgi.FieldStorage()

text = form.getvalue("text")

print("<html>")
print("<body>")
print("HELLO")
print("You entered: %s" % text)
print("</body>")
print("</html>")
