<?php
if(!isset($_COOKIE[$cookie_name]))
{
	$cookie_name = "Test";
	$cookie_value = "John Doe";

	setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/"); // 86400 = 1 day
}
?>
<html>
<body>

<?php
	if(!isset($_COOKIE[$cookie_name])) 
	{
		echo  "The variable '"  . $cookie_name . "\' is not set!<br>";
	}
	else 
	{
		echo "Cookie '" . $cookie_name . "' is set!<br>";
		echo "Value is: " . $_COOKIE[$cookie_name] ." <br>";
	}
?>

<a href="/
</body>
</html>