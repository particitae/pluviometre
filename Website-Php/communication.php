<?php
/**
 *  * Simple example of extending the SQLite3 class and changing the
 *  __construct
 *   * parameters, then using the open method to initialize the DB.
 *    */
class MyDB extends SQLite3
    {
    function __construct()
    {
        $this->open('mysqlitedb.db');
    }
}

function validateDate($date)
{
list($tempdate,$tempheure) = explode(' ', $date);
list($y, $m, $d) = explode('-', $tempdate);
list($heure,$minute) = explode(':', $tempheure);
if(!checkdate($m, $d, $y)){
	exit();
	}
if(!is_numeric($heure))
{
	exit();
}
if(!is_numeric($minute))
{
	exit();
}
return $date;
}
function validatePh($valeur)
{
    if (!is_numeric($valeur))
	{
	exit();
	}
   return str_replace(".",",",$valeur);
}

function validateId($valeur)
{
	if (!is_numeric($valeur))
	{
	exit();
	}
   return $valeur;
}
$dateenregistrement = validateDate(isset($_POST['date']) ? trim($_POST['date']) : '');
$identification = validateId(isset($_POST['identification']) ? trim($_POST['identification']) : '');
$ph = validatePh(isset($_POST['ph']) ? trim($_POST['ph']) : '');
$volume = validateId(isset($_POST['volume']) ? trim($_POST['volume']) : '');

date_default_timezone_set('Europe/Paris');
$date = new DateTime("now"); // 'now' n'est pas nécéssaire, c'est la valeur par défaut
$now = $date->format('Y-m-d H:i');

$db = new MyDB();
$db->exec("INSERT INTO `pluviometre` ( `identification`, `date`, `ph`, `volume`, `enregistrement`) VALUES ('$identification','$dateenregistrement', '$ph', '$volume','$now')");
?>
