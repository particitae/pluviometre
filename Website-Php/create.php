<?php
/**
 *  * Creation de la base de données et ajout d'une valeur de test
 *    */
class MyDB extends SQLite3
    {
    function __construct()
    {
        $this->open('mysqlitedb.db');
    }
}
$db = new MyDB();
$db->exec("CREATE TABLE `pluviometre` ( `id` INTEGER PRIMARY KEY AUTOINCREMENT, `identification` SMALLINT NOT NULL,  `date` DATETIME NOT NULL, `ph` FLOAT NOT NULL, `volume` INT NOT NULL, `enregistrement` DATETIME NOT NULL )");
$db->exec("INSERT INTO `pluviometre` ( `identification`, `date`, `ph`, `volume`, `enregistrement`) VALUES ( '001', '2999-01-01 00:00', '7,00', '999', '2999-01-01 00:00')");
?>
