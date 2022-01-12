<?php

echo "try";
if (!empty($_POST))
{
	echo 'Bonjour ' . $_POST["text1"] . htmlspecialchars($_POST["text2"]) . '!';
}
else
{
	echo "WTFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
}
// htmlspecialchars($_POST["file1"])

?>
