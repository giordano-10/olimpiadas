<?
$data = json_decode(file_get_contents("php://input"), true);
$servername = "localhost";
$username = "root";
$password = "";
$database = "hospital";

$conn = new mysqli($servername, $username, $password, $database);

// Verifica la conexión
if ($conn->connect_error) {
    die("Error de conexión: " . $conn->connect_error);
}

if ($data) {
    // Insertar datos en la base de datos
    $tiempo_respuesta = $data['valor'];
    
    $sql = "INSERT INTO datos_paciente (tiempo_respuesta) VALUES ('$tiempo_respuesta')";
    
    if ($conn->query($sql) === TRUE) {
        echo "Datos insertados correctamente";
    } else {
        echo "Error al insertar datos: " . $conn->error;
    }
} else {
    echo "Datos no recibidos correctamente";
}

$conn->close();
?>
