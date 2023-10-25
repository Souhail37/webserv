# webserv
This is when you finally understand why a URL starts with HTTP
# Chapter I: HTTP Server in C++ 98

This project involves building a simple HTTP server in C++ 98. The Hypertext Transfer Protocol (HTTP) is the foundation of data communication for the World Wide Web, and your server will be responsible for handling HTTP requests and responses.

## Table of Contents

- [Introduction](#introduction)
- [Mandatory Part](#mandatory-part)
- [Requirements](#requirements)
- [Configuration File](#configuration-file)

## Introduction

HTTP is an application protocol for distributed, collaborative, hypermedia information systems. It's used for communication between web servers and clients, enabling users to access hypertext documents, images, stylesheets, scripts, and more.

The primary function of a web server is to store, process, and deliver web pages to clients, and your HTTP server will do exactly that.
  
## Mandatory Part
  
### Description

You are tasked with building an HTTP server in C++ 98. Your executable will run as follows:
- ./webserv [configuration file]

### Requirements

Your HTTP server must meet the following requirements:

- Must be non-blocking and use only one `poll()` (or equivalent) for all I/O operations between the client and server.
- Your server should be compatible with common web browsers.
- Serve fully static websites.
- Support GET, POST, and DELETE methods.
- Clients must be able to upload files.
- Stress test your server to ensure it remains available.
- Listen on multiple ports as defined in the configuration file.

## Configuration File

You can configure your server using a configuration file. The configuration file should include settings for:

- Port and host for each server.
- Server names or hostnames.
- Default error pages.
- Limiting client body size.
- Defining routes with rules for HTTP methods, redirection, directory settings, directory listing, default files, CGI execution, and more.
