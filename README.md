# Terminal-based Chat Application

This is a simple terminal-based group chat application implemented in C++.

## Features

- **User :** Users have to provide their name to start the chat.
- **Real-time Messaging:** Users can send and receive messages in real-time.
- **User List:** Multiple users can join the chat at a time.
- **Simple Interface:** Easy-to-use command line interface.
- **Disadvantage:** Only is only for windows.

## Installation

1. Clone this repository to your local machine:

    ```bash
    git clone https://github.com/a4appIe/cpp-chat-app.git
    ```

2. Navigate to the directory:

    ```bash
    cd cpp-chat-app
    ```

3. Compile server code:

    ```bash
    g++ -o server server.cpp
    ```

4. Compile client code:

    ```bash
    g++ -o client client.cpp
    ```

## Usage 
### Run these commands in different terminal/command prompt tabs.

1. Run the server executable:

    ```bash
    server.exe
    ```
2. Run the client executable:

    ```bash
    client.exe
    ```

2. You will be prompted to enter your name for the chat. Enter your name.

3. Once entered, you can start sending and receiving messages with other online users.

4. To exit the application, simply type `/quit`.

## Dependencies

- C++ Standard Library
- Winsock2
- WS2tcpip
- tchar.h
- thread
- vector

## Contributing

Contributions are welcome! If you  want to add new features, feel free to submit a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
