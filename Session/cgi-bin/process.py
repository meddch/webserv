#!/usr/bin/env python3
import cgi

print("Content-Type: text/html")

print()  # blank line, end of headers

form = cgi.FieldStorage()
name = form.getvalue("name")
email = form.getvalue("email")
phone_number = form.getvalue("phone_number")
message = form.getvalue("message")

print("""
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>User Info</title>
</head>
<body>
    <h1>User Info</h1>
    <p>Name: {}</p>
    <p>Email: {}</p>
    <p>Phone Number: {}</p>
    <p>Message: {}</p>
</body>
</html>
""".format(name, email, phone_number, message))
exit(0)
