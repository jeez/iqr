use IO::Socket;
my $sock = new IO::Socket::INET (
PeerAddr => 'localhost',
PeerPort => '54923',
Proto => 'tcp',
);
die "Error: $!\n" unless $sock;
print $sock "cmd:stop\n";
close($sock);
