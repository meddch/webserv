**# Webserv42: Craft Your Own HTTP Server in C++**

Explore the Depths of HTTP and Master Web Server Development.

Welcome to Webserv42, your gateway to hands-on HTTP server creation in C++! This repository empowers you to construct a fully functional server, unlocking a deep understanding of web technologies and hands-on development experience.

**Key Features:**

- **Customizable Configuration:** Adapt the server to your exact needs using a flexible configuration file.
- **Efficient Handling of Multiple Connections:** Ensure seamless multi-user experiences with kqueue-based I/O multiplexing.
- **Dynamic Content Generation:** Bring your server to life with CGI/1.1 support, enabling dynamic responses.
- **Modern Web Standards Compliance:** Embrace HTTP/1.1 to ensure compatibility with current web technologies.
- **Comprehensive File Management:** Serve static content (GET), handle file uploads (POST), and implement file deletion (DELETE) with ease.

**Quick Start:**

1. **Build:** Effortlessly compile the server using `make`.
2. **Launch:** Start the server with `./webserv config/`, specifying the path to your configuration file.

**Configuration Options:**

- **Hostname:** Set the primary domain for your server.
- **Ports:** Designate the ports on which the server will listen for requests.
- **Directory Listings:** Customize how directory contents are presented.
- **Default Files:** Specify index files to serve when a directory is requested.
- **Routing:** Define precise rules for handling different directories and file extensions.
- **HTTP Method Control:** Grant or restrict access to specific HTTP methods (GET, POST, DELETE) for security.
- **CGI Scripting:** Configure the server to execute CGI scripts, enabling dynamic content generation.
- **Maximum Request Size:** Set limits for individual files or chunks received in requests.
- **Virtual Server Support:** Host multiple websites on a single server instance through distinct configuration blocks.

**Embark on Your HTTP Server Development Journey Today!**

Clone this repository, dive into the code, and construct your own powerful HTTP server in C++. Unlock the secrets of web technologies and gain invaluable hands-on experience!
