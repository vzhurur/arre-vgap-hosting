<?php
function upload($post, $files, $player) {
    $target_dir = $upload_dir;
    $file_name = basename($files["fileToUpload"]["name"]);
    $target_file = $target_dir . $file_name;
    $uploadOk = 1;
    $FileType = pathinfo($target_file,PATHINFO_EXTENSION);
    
    if( ! isset($post["submit"])) {
        echo "Not submitting, should be imposible to get here.";
        $uploadOk = 0;
    }
   
    // Check file size
    if ($files["fileToUpload"]["size"] > 500000) {
        echo "Sorry, your file is too large.";
        $uploadOk = 0;
    }
    
    // Allow certain file formats
    if ($FileType != "trn" && $FileType != "TRN" ) {
        echo "Sorry, only TRN files are allowed.";
        $uploadOk = 0;
    }
    
    $required_filename = "player" . $player . ".trn";
    if ( 0 != strcasecmp( $file_name, $required_filename ) {
        echo "Sorry, wrong file for player number";
        $uploadOk = 0;
    } 
    
    if ( ! is_uploaded_file($_FILES['fileToUpload']['tmp_name']) ) {
        echo "Sorry, is_uploaded_file fails";
        $uploadOk = 0;
    } 
    
    // Check if $uploadOk is set to 0 by an error
    if ($uploadOk == 0) {
        echo "Sorry, your file was not uploaded.";
    // if everything is ok, try to upload file
    } else {
        if (move_uploaded_file($files["fileToUpload"]["tmp_name"], $target_file)) {
            echo "The file ". $file_name . " has been uploaded.";
        } else {
            echo "Sorry, there was an error moving your file from '" . $files["fileToUpload"]["tmp_name"] . "' to '" . $target_file . "'";
        }
    }
}

?>
