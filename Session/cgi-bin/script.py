#!/usr/bin/env python3

import os
import sys
import urllib.parse
import cgi
import http.cookies

# Check if the request method is set, otherwise default to 'CLI'
method = os.environ.get('REQUEST_METHOD', 'CLI')

# Read data from stdin
data = sys.stdin.read()

# Parse the data
output = urllib.parse.parse_qs(data)

# Get the name, or default to 'Guest'

name = output.get('name', 'Guest')

# Create a new cookie
cookie = http.cookies.SimpleCookie()
cookie["name"] = name
cookie["name"]["path"] = "/" 

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