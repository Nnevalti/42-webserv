<?php

	if (isset($_GET['name'])) {
		echo "Name: " . $_GET['name'] . "\n";
		echo "Age: " . $_GET['age'] . "\n";
	}
	else {
		echo "Nothing";
	}
?>
