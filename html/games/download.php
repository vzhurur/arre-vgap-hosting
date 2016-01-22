<?php
require 'game_slots.php';

$player = $datos[$_SERVER['PHP_AUTH_USER']][0];
$file = $download_dir . "player" . $player . ".tar";

if (file_exists($file)) {
    header('Content-Description: File Transfer');
    header('Content-Type: application/octet-stream');
    header('Content-Disposition: attachment; filename='.basename($file));
    header('Expires: 0');
    header('Cache-Control: must-revalidate');
    header('Pragma: public');
    header('Content-Length: ' . filesize($file));
	readfile($file);
}
else {
	echo "Error: No se puede encontrar el archivo: '" . $file . "' con el turno";
}

?>
