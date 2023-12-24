Mail
1.Compile file in the linux
  (1)update
    sudo apt install g++ build-essential
    sudo apt install vim
  (2)compile
    g++ -c fileName.cpp -o fileName.o
    ex: g++ -c mail.cpp -o mail.o
    
    g++ fileName.o -o fileName
    ex: g++ mail.o -o mail

2.run
  (1) run file
    ./fileName
    ex: ./mail

  (2) input 
    sender's IP address
    ex: 172.21.34.66

    sender's email
    ex: LAPTOP-EDPLP3A3@bell.inv.tw

    recipient's email
    ex: B11130038@mail.ntust.edu.tw

    email title
    ex: test

    email content(end with <Ctrl+D>)
    ex: send the email

Iperfer
1.Compile file in the linux
  (1)update
    sudo apt install g++ build-essential
    sudo apt install vim
  (2)compile
    g++ -c fileName.cpp -o fileName.o
    ex: g++ -c Iperfer.cpp -o Iperfer.o
    
    g++ fileName.o -o fileName
    ex: g++ Iperfer.o -o Iperfer

2.run
  (1) server
    fileName -s -p <listen port>
    ex: Iperfer -s -p 48763
  (2) client
    fileName -c -h <server hostname> -p <server port> -t <time>
    ex: Iperfer -c -h 172.21.34.66 -p 487632 -t 3

3.precaution
  (1)the port should be in the range 1024 ≤ server port ≤ 65535
  (2)each element is must, or it will report error

