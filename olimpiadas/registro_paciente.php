<?php
// Configura la conexión a la base de datos
$servername = "localhost";
$username = "root";
$password = "";
$database = "hospital";

$conn = new mysqli($servername, $username, $password, $database);

// Verifica la conexión
if ($conn->connect_error) {
    die("Error de conexión: " . $conn->connect_error);
}

// Obtiene los datos del formulario
$nombre = $_POST['nombre'];
$apellido = $_POST['apellido'];
$fecha_nacimiento = $_POST['documento'];
$genero = $_POST['genero'];
$enfermedad_actual = $_POST['enfermedad'];
$medicamentos_actuales = $_POST['estado'];
$direccion = $_POST['area'];
$ciudad = $_POST['cuarto'];
$enfermero_asignado = $_POST['enfermero'];
$enfermero_id = $_POST['enfermero_id'];

// Inserta los datos en la base de datos
$sql = "INSERT INTO datos_paciente (nombre, apellido, document, genero, enfermedad_actual, estado_actual, area, cuarto, enfermero, enfermero_id) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";

if ($stmt = $conn->prepare($sql)) {
    $stmt->bind_param("sssssssssss", $nombre, $apellido, $fecha_nacimiento, $genero, $enfermedad_actual, $medicamentos_actuales, $alergias, $direccion, $ciudad, $estado, $enfermero_asignado);

    if ($stmt->execute()) {
        echo "Los datos se han ingresado correctamente en la base de datos.";
    } else {
        echo "Error al insertar datos: " . $stmt->error;
    }

    $stmt->close();
} else {
    echo "Error en la preparación de la consulta: " . $conn->error;
}

$conn->close();
?>
