<?php

// Get the session ID from the HTTP_COOKIE environment variable
$sessionId = $_ENV['SESSION_ID'] ?? null;
$csvFile = './data.csv';

$queryString = $_ENV['QUERY_STRING'] ?? null;
if ($queryString !== null) {
    parse_str($queryString, $output);
	$name = isset($output['name']) && $output['name'] !== '' ? $output['name'] : null;
    $login = isset($output['login']) && $output['login'] !== '' ? $output['login'] : null;
    if ($name !== null && $login !== null) {
        // Assign new ID and list them in the CSV
        $sessionId = uniqid();
        $newData = $sessionId . ',' . $queryString . "\n";
        file_put_contents($csvFile, $newData, FILE_APPEND);

        $name = $output['name'];
        $login = $output['login'];
    } elseif ($sessionId !== null) {
        // If the session ID exists, retrieve the data from the CSV file
        $csvData = file_get_contents($csvFile);
        $matchingRow = null;
        if ($csvData !== false) {
            $rows = explode("\n", $csvData);
            foreach ($rows as $row) {
                $rowData = str_getcsv($row);
                if ($rowData[0] === $sessionId) {
                    $matchingRow = $rowData;
                    break;
                }
            }
        }

        if ($matchingRow !== null) {
            $queryString = $matchingRow[1];
            parse_str($queryString, $output);
            $name = isset($output['name']) ? $output['name'] : 'Guest';
            $login = isset($output['login']) ? $output['login'] : 'Guest';
        } else {
            $name = 'Guest';
            $login = 'Guest';
        }
    } else {
        $name = 'Guest';
        $login = 'Guest';
    }
} else {
    $name = 'Guest';
    $login = 'Guest';
}

echo "Content-Type: text/html";
if ($sessionId !== null  && $name !== 'Guest' && $login !== 'Guest') {
    echo "\r\n";
    echo "set-cookie: session_id=" . $sessionId;
}
echo "\r\n\r\n";

echo "<html><head><style>";
echo "body { background-color: #1a1a1a; color: #ffd700; font-family: 'Courier New', Courier, monospace; margin: 0; display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100vh; }";
echo "h1 { font-size: 77px; margin-bottom: 20px; margin: 117px; }";
echo "</style></head><body>";
echo "<h1>Hello, " . $name . "!</h1>";
echo "<h2>aka: " . $login . "</h2>";
echo "</body></html>";
?>