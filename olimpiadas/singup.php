<?php
$usuario = $_POST["user"];
$password2 = $_POST["password"];

$servername = "localhost";
$username = "root";
$password = "";
$database = "prueba";

$conn = new mysqli($servername, $username, $password, $database);

// Verificar la conexión a la base de datos
if ($conn->connect_error) {
    die("Conexión fallida: " . $conn->connect_error);
}

// Escapar los valores del formulario para prevenir inyección de SQL
$usuario = $conn->real_escape_string($usuario);
$password2 = $conn->real_escape_string($password2);

// Consulta SQL para verificar si el usuario ya existe en la tabla de usuarios
$sql = "SELECT * FROM usuarios WHERE user = '$usuario' AND password = '$password2'";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // El usuario ya existe
    echo "<script>alert('El nombre de usuario ya está registrado');window.location.href = '/public/login.html';</script>";
    
} else {
    if (strlen($usuario) < 8 || strlen($usuario) > 20) {
        echo "<script>alert('El nombre de usuario debe tener entre 8 y 20 caracteres.');window.location.href = '/public/login.html'</script>";
        exit;
    }
    // Validación de la contraseña (más de 8 caracteres y al menos un número)
    if (strlen($password2) <= 8 || !preg_match('/\d/', $password2)) {
        echo "<script>alert('La contraseña debe tener más de 8 caracteres y al menos un número.');window.location.href = '/public/login.html'</script>";
        exit;
    } else {
        // Insertar el nuevo usuario en la tabla de usuarios
        $sql = "INSERT INTO usuarios (user, password) VALUES ('$usuario', '$password2')";
        if ($conn->query($sql) === true) {
            // Registro exitoso
            echo "<script>alert('Registro exitoso. Ahora puedes iniciar sesión.');window.location.href = '/public/login.html';</script>";
        } else {
            // Error al insertar en la base de datos
            echo "<script>alert('Error al registrar el usuario: " . $conn->error . "');window.location.href = '/public/login.html';</script>";
        }
    }
}

// Cerrar la conexión a la base de datos
$conn->close();
?>
