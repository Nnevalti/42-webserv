<HTML>
		<HEAD>
			<TITLE>About this Server</TITLE>
		</HEAD>
		<BODY>
			<H1>About this Server</H1>
		<HR><PRE>
		Server Name:      <?php echo $_ENV["SERVER_NAME"] ?> <br>
		Running on Port:  <?php echo $_ENV["SERVER_PORT"] ?> <br>
		Server Software:  <?php echo $_ENV["SERVER_SOFTWARE"] ?> <br>
		Server Protocol:  <?php echo $_ENV["SERVER_PROTOCOL"] ?> <br>
		CGI Revision:     <?php echo $_ENV["GATEWAY_INTERFACE"] ?> <br>
		<HR></PRE>
	</BODY>
</HTML>
