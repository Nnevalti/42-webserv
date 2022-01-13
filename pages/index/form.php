<?php

if($_SERVER['REQUEST_METHOD'] == 'POST')
{
	echo 'Request was a POST';
}
if (!empty($_POST))
{
	echo 'Bonjour ' . $_POST["text1"] . htmlspecialchars($_POST["text2"]) . "!\n";
}
else
{
	echo "\$_POST is empty\n";
}

if (!empty($_FILE))
{
	// UPLOAD
	$uploaddir = __DIR__ . '/upload/';
	$uploadfile = $uploaddir . basename($_FILES['userfile']['name']);
	$tmp_name = $_FILES['userfile']['tmp_name'];
	echo $tmp_name;
	echo '<pre>';
	if (move_uploaded_file($tmp_name, $uploadfile)) {
		echo "Le fichier est valide, et a été téléchargé
			   avec succès. Voici plus d'informations :\n";
	} else {
		echo "Attaque potentielle par téléchargement de fichiers.
			  Voici plus d'informations :\n";
	}

	echo 'Voici quelques informations de débogage :';
	print_r($_FILES);
}
else
{
	echo "\$_FILE is empty\n";

}

?>
