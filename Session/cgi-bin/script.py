#!/usr/bin/env python3

import cgi
import http.cookies
import os
import sys
import urllib.parse

# Check if the request method is set, otherwise default to 'CLI'
method = os.environ.get('REQUEST_METHOD', 'CLI')

# Read data from stdin
data = sys.stdin.read()

# Parse the data
output = urllib.parse.parse_qs(data)

# Get the HTTP_COOKIE environment variable
cookie_string = os.environ.get('HTTP_COOKIE', '')

# Parse the cookies
cookie = http.cookies.SimpleCookie()
cookie.load(cookie_string)

# Get the 'name' value from the parsed data or the cookie
name = output.get('name', [cookie['name'].value if 'name' in cookie else 'Guest'])[0]

# Create a new cookie
cookie = http.cookies.SimpleCookie()
cookie["name"] = name

# Print the headers
print('Content-Type: text/html; charset=utf-8')
print(cookie.output())
print()

# Print the HTML
print(f"""
<html>
<head>
<style>
body {{ background-color: #f0f0f0; font-family: Arial, sans-serif; }}
h1 {{ color: #333; }}
</style>
</head>
<body>
<h1>Hello, {name}!</h1>
</body>
</html>
""")