make:
	g++ -std=c++11 -I/usr/local/Cellar/openssl/1.0.2k/include -L/usr/local/Cellar//openssl@1.1/1.1.1i/lib/ -lssl -lcrypto -o main main.cpp block.cpp chain.cpp hash.cpp trans.cpp uuid.cpp user.cpp `pkg-config --cflags --libs libbitcoin` -w
run:
	./main