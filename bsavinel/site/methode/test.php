<?php
if(!isset($_COOKIE[$cookie_name]))
{
	$cookie_name = "user";
	$cookie_value = "John Doe";
	$test_cookie2_name = "test";
	$test_cookie2_value = "VALUE";

	setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/"); // 86400 = 1 day
	setcookie($test_cookie2_name, $test_cookie2_value, time() + (86400 * 30), "/"); // 86400 = 1 day
}
?>
<html>
<body>

<?php
	if(!isset($_COOKIE[$cookie_name])) {
	echo "Cookie named '" . $cookie_name . "' is not set!";
	} else {
	echo "Cookie '" . $cookie_name . "' is set!<br>";
	echo "Value is: " . $_COOKIE[$cookie_name] ." <br>";
	}
	if(!isset($_COOKIE[$test_cookie2_name])) {
		echo "Ca pue si tu lis ca<br>";
		} else {
		echo "Si tu lis cette phrase, t es bien<br>";
		echo "Value is: " . $_COOKIE[$test_cookie2_name];
		}
?>
</body>
</html>

