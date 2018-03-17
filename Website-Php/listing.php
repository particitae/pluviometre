<?php
/**
 *  * Affichage Brute des valeurs
 *    */
class MyDB extends SQLite3
    {
    function __construct()
    {
        $this->open('mysqlitedb.db');
    }
}
$db = new MyDB();
$result = $db->query('SELECT * FROM pluviometre');
while ($row = $result->fetchArray(SQLITE3_ASSOC)) {
   echo $row['id']."|".$row['identification'] . "|". $row['date'] . "|" . $row['ph']. "|" . $row['volume'] . "|" . $row['enregistrement'] . "<br/>"; 
}
?>
