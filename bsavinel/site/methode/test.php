<?php
if(!isset($_COOKIE[$cookie_name]))
{
	$cookie_name;
	$cookie_value;
	echo "<form action="methode/test.php" method="post"></form>
	<label for="fname">First name:</label>
	<input type="text" id=$cookie_name name="fname"><br><br>
	<label for="fname">First name:</label>
	<input type="text" id=$cookie_value; name="fname"><br><br>"


	setcookie($cookie_name, $cookie_value, time() + (86400 * 30), "/"); // 86400 = 1 day
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
?>
</body>
</html>

