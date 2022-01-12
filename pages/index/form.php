<?php

echo "try\n";
if (!empty($_POST))
{
	echo 'Bonjour ' . $_POST["text1"] . htmlspecialchars($_POST["text2"]) . "!\n";
	// UPLOAD
	$uploaddir = '/mnt/nfs/homes/vdescham/Documents/42-webserv/pages/index/upload/';
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

	echo '</pre>';

}
else
{
	echo "WTFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";
}
// htmlspecialchars($_POST["file1"])

?>
