<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Check if files were uploaded without errors
    if (isset($_FILES["files"])) {
        $targetDir = "/Users/mechane/Desktop/webser";
        $uploadedFiles = [];

        foreach ($_FILES["files"]["name"] as $key => $name) {
            $targetFile = $targetDir . basename($name);

            // Check if the file already exists
            if (file_exists($targetFile)) {
                echo "Sorry, the file $name already exists.<br>";
            } else {
                // Move the uploaded file to the specified directory
                if (move_uploaded_file($_FILES["files"]["tmp_name"][$key], $targetFile)) {
                    echo "The file $name has been uploaded.<br>";
                    $uploadedFiles[] = $name;
                } else {
                    echo "Sorry, there was an error uploading your file $name.<br>";
                }
            }
        }

        // Print the names of the uploaded files
        if (!empty($uploadedFiles)) {
            echo "Uploaded files: " . implode(", ", $uploadedFiles);
        }
    } else {
        echo "No files were uploaded.";
    }
}
?>