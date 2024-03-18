# webserv

## Workflow 1: Server Core & I/O Multiplexing
### Assigned to: Team Member A

#### Tasks:
1. **Implement Server Core:**
   - Establish TCP socket listening.
   - Manage incoming connections and client sessions.
   - Handle data transmission between server and clients.

2. **Develop I/O Multiplexing:**
   - Implement `epoll()` for multiplexing.
   - Ensure non-blocking and efficient data handling.

#### Deliverable:
- A functional Server Core capable of handling multiple connections.

---

## Workflow 2: Request Parser & Response Builder
### Assigned to: Team Member B

#### Tasks:
1. **Develop Request Parser:**
   - Build HttpHandler class for HTTP request parsing.
   - Manage parsing of request components (method, path, headers, body).
   - Handle parsing errors and incomplete requests.

2. **Implement Response Builder:**
   - Create Response class for HTTP response construction.
   - Manage status line, headers, and message body in responses.
   - Ensure appropriate response generation based on request analysis.

#### Deliverable:
- A fully operational Request Parser and Response Builder.

---

## Workflow 3: Configuration File & CGI
### Assigned to: Team Member C

#### Tasks:
1. **Create Configuration File Parser:**
   - Develop parser for server configuration file.
   - Implement handling for various directives (e.g., listen, host, root).
   - Enable dynamic server behavior based on configuration settings.

2. **Integrate CGI Support:**
   - Implement CGI functionality for executing external scripts.
   - Establish data transfer mechanism between server and CGI scripts.
   - Handle diverse script types (Python, Bash, etc.).

#### Deliverables:
- A mechanism for handling configuration files.
- Functional integration of CGI.
