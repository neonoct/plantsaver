<?php
class dd{
 public $link='';
 function __construct($Light, $xx){
  $this->connect();
  $this->storeInDB($Light, $xx);
 }
 
 function connect(){
  $this->link = mysqli_connect('localhost','root','') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'ldrsens') or die('Cannot select the DB');
 }
 
 function storeInDB($Light, $xx){
  $query = "insert into ldr set xx='".$xx."', Light='".$Light."'";
  $result = mysqli_query($this->link,$query) or die('Errant query:  '.$query);
 }
 
}
if($_GET['Light'] != '' and  $_GET['xx'] != ''){
 $dht11=new dd($_GET['Light'],$_GET['xx']);
}


?>