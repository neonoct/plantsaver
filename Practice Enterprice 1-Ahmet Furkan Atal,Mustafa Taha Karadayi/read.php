<?php


$dbusername = "root"; 
$server = "localhost"; 
$dbname = "ldrsens"; 

// Create connection
$conn = new mysqli($server, $dbusername,'', $dbname);
// Check connection
if ($conn->connect_error) {
die("Connection failed: " . $conn->connect_error);
}else{

} 

$sql = "SELECT Avg(Light) as Mean FROM ldr  where lightTime > date_sub(now(), interval 1.4 minute)"; //normally 14 hours but for demo 1.4minute
$result = $conn->query($sql);

if ($result->num_rows > 0) {
// output data of each row
while($row = $result->fetch_assoc()) {
echo  $row["Mean"];
echo "\n ";





}
} else {
echo "No result";
}
$conn->close();
?>