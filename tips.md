### http_server_app
g++ -o http-server http_server_app.cpp -lboost_filesystem -lboost_system


### ssl/tls
g++ -o sync_tcp_ssl_iterative_server sync_tcp_ssl_tls_iterative_server.cpp -lcrypto -lssl
g++ -o sync_tcp_ssl_client sync_tcp_ssl_tls_client.cpp -lcrypto -lssl

//生成私钥
openssl genrsa -des3 -out server.key 2048

//生成证书签名请求
openssl req -new -key server.key -out server.csr

//用私钥签名证书
openssl x509 -req -days 3650 -in server.csr -signkey server.key -out server.crt

//删除密码要求(例如，需要)
cp server.key server.key.secure
openssl rsa -in server.key.secure -out server.key

//生成dhparam文件
openssl dhparam -out dh512.pem 512

#### 完成此操作后，您需要更改server.cpp和client.cpp中的文件名。

server.cpp
context_.use_certificate_chain_file("server.crt");
context_.use_private_key_file("server.key", boost::asio::ssl::context::pem);
context_.use_tmp_dh_file("dh512.pem");

client.cpp
ctx.load_verify_file("server.crt");
