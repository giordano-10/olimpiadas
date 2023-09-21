<!DOCTYPE html>
<html>
<head>
<link rel="stylesheet" href="estilo_vistaArea.css">
    <title>Buscador de Datos Hospitalarios</title>
</head>
<body>
<h1>Buscador de Datos Hospitalarios</h1>
    <form method="post">
        <label for="busqueda">Ingresa un Paciente:</label>
        <input type="text" name="busqueda">
        <br><br>
        <input type="submit" value="Buscar">
    </form>

    <!-- Resultados -->
    <?php
    // Verifica si se ha enviado el formulario
    if ($_SERVER["REQUEST_METHOD"] == "POST") {
        // Recupera la palabra clave ingresada
        $busqueda = $_POST['busqueda'];

        // Conecta a la base de datos (debes configurar las credenciales)
        $servername = "localhost";
        $username = "root";
        $password = "";
        $database = "hospital";

        $conn = new mysqli($servername, $username, $password, $database);

        // Verifica la conexión
        if ($conn->connect_error) {
            die("Error de conexión: " . $conn->connect_error);
        }

        // Realiza la consulta en función de la palabra clave
        $sql = "SELECT numero_cuarto, paciente, numero_paciente FROM areas WHERE paciente LIKE '%" . $busqueda . "%'";

        $result = $conn->query($sql);

        // Muestra los resultados
        if ($result->num_rows > 0) {
            echo "<h2>Resultados para la búsqueda: '" . $busqueda . "':</h2>";
            echo "<ul>";

            while ($row = $result->fetch_assoc()) {
                echo "<li>Número de Cuarto: " . $row['numero_cuarto'] . "</li>";
                echo "<li>Paciente: " . $row['paciente'] . "</li>";
                echo "<li>Número de Paciente: " . $row['numero_paciente'] . "</li>";
                echo "<hr>";
            }

            echo "</ul>";
        } else {
            echo "<p>No se encontraron resultados para la búsqueda: '" . $busqueda . "'.</p>";
        }

        // Cierra la conexión a la base de datos
        $conn->close();
    }
    ?>
</body>
</html>
