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
if name != "Guest":
	cookie = http.cookies.SimpleCookie()
	cookie["name"] = name
	cookie["name"]["path"] = "/cgi-bin/script.py" 
# Print the headers
print('Content-Type: text/html; charset=utf-8')
print(cookie.output())
print("\r\n\r\n")


print(f"""
<html>
<head>
	<style>
		body {{
			background-color: #1a1a1a; 
			color: #ffd700; 
			font-family: "Courier New", Courier, monospace;
			margin: 0;
			display: flex;
			flex-direction: column;
			align-items: center;
			justify-content: center;
			height: 100vh;
		}}

		header {{
			font-size: 77px; 
			font-weight: bold;
			margin-bottom: 20px;
			margin-top: 10%;
		}}

		#welcome-message {{
			font-size: 17px;
			margin-bottom: 20px;
		}}

		.button-container {{
			display: flex;
		}}

		.button {{
			display: inline-block;
			padding: 15px 30px;
			margin: 10px;
			text-align: center;
			text-decoration: none;
			color: #1a1a1a; 
			background-color: #ffd700; 
			border: 2px solid #ffd700; 
			border-radius: 5px;
			transition: background-color 0.3s ease;
		}}

		.button:hover {{
			background-color: #1a1a1a;
			border: 2px solid #1a1a1a;
			color: #ffd700;
		}}

		#developer-info {{
			font-size: 14px;
			margin-top: 20%;
			text-align: center;
			color: #ffd700;
		}}
	</style>
</head>
<body>

	<header>Webserv</header>

	 <p>Hi "{name}" welcome to our Webserv !</p> 
	<div class="button-container">

		<a href="/upload.html" class="button">Upload</a>
		<a href="/form.html" class="button">Form</a>
		<a href="/media.html" class="button">Media</a>
		<a href="/team.html" class="button">Team</a>
		<a href="/welcome.html" class="button">Go Back</a>
	</div>

	<div id="developer-info">
		Webserv/1.0<br>
		by: mechane-azari
	</div>

</body>
</html>
""")