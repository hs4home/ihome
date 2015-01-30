<?php

include 'crc.php';
include 'tcp_defines.php';


error_reporting(E_ALL);

/* Allow the script to hang around waiting for connections. */
set_time_limit(0);

/* Turn on implicit output flushing so we see what we're getting
 * as it comes in. */
ob_implicit_flush();

$address = '192.168.0.109';
$port = 111;

if (($sock = socket_create(AF_INET, SOCK_STREAM, SOL_TCP)) === false) {
    echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . "\n";
}

if (socket_bind($sock, $address, $port) === false) {
    echo "socket_bind() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
}

if (socket_listen($sock, 5) === false) {
    echo "socket_listen() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
}

echo "Server started\n"; 

do {
    if (($msgsock = socket_accept($sock)) === false) {
        echo "socket_accept() failed: reason: " . socket_strerror(socket_last_error($sock)) . "\n";
        break;
    }
	
	echo "Incoming connection\n"; 
	


    if (false === ($buf = socket_read($msgsock, 50, PHP_BINARY_READ))) {
		echo "socket_read() failed: reason: " . socket_strerror(socket_last_error($msgsock)) . "\n";
		break 2;
	}
	
	echo bin2hex($buf) . "\n"; 



	$talkback = pack("CCn",CID_RELAY,REP_RSTATE_ACK,0);
	
	socket_write($msgsock, $talkback, strlen($talkback));


	socket_close($msgsock);
} while (true);

socket_close($sock);
?>