<?php

$queryString = $_ENV['QUERY_STRING'];
parse_str($queryString, $output);

$name = isset($output['name']) ? $output['name'] : 'Name not provided';
$login = isset($output['login']) ? $output['login'] : 'Login not provided';


header('Content-Type: text/html; charset=utf-8');
echo "\n";
echo "<html><head><style>";
echo "body { background-color: #1a1a1a; color: #ffd700; font-family: 'Courier New', Courier, monospace; margin: 0; display: flex; flex-direction: column; align-items: center; justify-content: center; height: 100vh; }";
echo "h1 { font-size: 77px; margin-bottom: 20px; margin: 117px; }";
echo "</style></head><body>";
echo "<h1>Hello, " .$name . "!</h1>";
echo "<h2>aka : " .$login. "</h2>";
echo "</body></html>";
?>