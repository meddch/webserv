<?php
$method = $_SERVER['REQUEST_METHOD'] ?? 'CLI';
if ($method === 'GET') {
	$data = $_SERVER['QUERY_STRING'] ?? '';
} elseif ($method === 'POST') {
	$data = file_get_contents('php://stdin');
}

parse_str($data, $output);
$name = $output['name'] ?? 'Guest';



header('Content-Type: text/html; charset=utf-8');
echo "Set-Cookie: name=$name; expires=Fri, 31-Dec-2021 23:59:59 GMT; path=/\n";
echo "\n";
echo "<html><head><style>";
echo "body { background-color: #f0f0f0; font-family: Arial, sans-serif; }";
echo "h1 { color: #333; }";
echo "</style></head><body>";
echo "<h1>Hello, $name!</h1>";
echo "</body></html>";
// while(1){echo "Hello, $name!\n";}
?>