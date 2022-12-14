#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include <netinet/in.h>
#include <map>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <list>
#include <thread>
#include <mutex>
#include <future>
#include <gtk/gtk.h>
#include "Cuarto.h"
#include "Procesador_Servidor.h"
#include "Fabrica_Procesadores.h"

/**
 * Clase que encapsula el comportamiento de un servidor
 * que administrará el chat.
 *
 */
class Servidor {
  
  /** El puerto al que se enlazará el servidor. */
  int puerto;
  /** El entero que representa el socket del servidor. */
  int file_descriptor;
  /** El entero que representa el estado de la operación
      de lectura del servidor. */
  int read_status;
  /** El entero que representa el estado de la operación
      de escritura del servidor. */  
  int write_status;
  /** El objeto que guarda los mensajes de aviso emitidos
      por el servidor. */
  GtkTextBuffer* text_buffer;
  
  /** Objeto que representa al servidor. */
  struct sockaddr_in direccion_servidor;

  /** Objeto que representa un cliente. */
  struct sockaddr_in direccion_cliente;
  /** Mutex para hilos de ejecución */
  std::mutex client_mutex, send_mutex;

  /** Arreglo que almacenará los datos recibidos de los clientes. */
  char host_buffer[1024];
  /** Registra el número de clientes; además, funge como identificador
      de los mismos. */
  int seed = 0;
  
  Procesador_Servidor procesador;
  
  /**
   * Diccionarios que almacenarán la información de los usuarios
   * conectados y las salas creadas, a partir de una cadema
   * representante de su nombre.
   *
   */
  std::map<std::string, Usuario*> usuarios;
  std::map<std::string, Cuarto*> cuartos;
  
public:

  Servidor(int puerto);

  int get_file_descriptor();

  char* get_host_buffer();

  /**
   * Devuelve el número de clientes conectados
   * al servidor.
   * @return El número de clientes conectados
   * al servidor.
   *
   */
  int get_seed();

  /**
   * Devuelve el entero que representa el estado
   * de la operación de lectura del servidor.
   * @return 0, si no ocurrieron problemas durante
   * la ejecución; -1, en otro caso.
   *
   */
  int get_read_status();

  /**
   * Devuelve el entero que representa el estado
   * de la operación de escritura del servidor.
   * @return 0, si no ocurrieron problemas durante
   * la ejecución; -1, en otro caso.
   *
   */
  int get_write_status();

  /**
   * Actualiza en una unidad el número de clientes
   * conectados al servidor.
   *
   */
  void set_seed();

  /**
   * Define el estado de la operación de lectura
   * del servidor.
   * @param status 0, si no ocurrieron problemas durante
   * la ejecución; -1, en otro caso.
   *
   */
  void set_read_status(int status);

  /**
   * Define el estado de la operación de escritura
   * del servidor.
   * @param status 0, si no ocurrieron problemas durante
   * la ejecución; -1, en otro caso.
   *
   */
  void set_write_status(int status);

  /**
   * Define el objeto que guarda los mensajes de aviso emitidos
   * por el servidor.
   * @param text_buffer El objeto que guarda los mensajes de aviso
   * emitidos por el servidor.
   *
   */
  void set_text_buffer(GtkTextBuffer* text_buffer);
  
  /*
  * Devuelve un descriptor de archivo de socket o -1,
  * en caso de error.
  */
  int servidor_socket();

  int servidor_bind();

  int servidor_listen(int tamano_cola);

  /*
  * Devuelve un nuevo descriptor de archivo de socket o -1,
  * en caso de error.
  * @return el nuevo descriptor de archivo o -1;
  */
  int servidor_accept();

  /*
  * Envía un arreglo de caracteres al socket parámetro.
  * @param descriptor_archivo el socket cliente
  * @param message el mensaje a enviar
  */
  int servidor_send(int descriptor_archivo, char * message);

  /*
  * Lee los datos del soket servidor.
  * @param descriptor_archivo el socket 
  *
  */
  int servidor_read(int descriptor_archivo);

  /*
  * Cierra el descriptor de archivo de socket parámetro.
  */
  void servidor_close(int descriptor_archivo);

  /** Envía un mensaje global a los usuarios conectados */
  void global_message(std::string message);

  /** Envía un mensaje global a los usuarios conectados */
  void global_message_from(std::string message, int sender_id);

  /** Envía un mensaje a un usuario en concreto */
  void send_message_to(std::string message, int socket);

  /**
   * Se ancargará de la administración del cliente
   * aceptado; incorporándolo a la lista de usuarios
   * registados y escuchando las peticiones que
   * este envíe.
   * @param client_socket El socket del cliente.
   * @param id El identificador del cliente.
   * @param usuario El usuario asociado al socket.
   * @return 0, si su ejecución no tuvo ningún error;
   * -1, en otro caso.
   *
   */
  void administra_cliente(Usuario* usuario);

  /**
   * Crea los hilos para la administración de los clientes.
   * @param client_socket El socket del cliente.
   * @param seed El identificador del cliente.
   * @param usuario El usuario asociado al socket.
   * @return El hilo que se encargará de administrar el
   * cliente.
   *
   */
  std::thread crea_hilo(Usuario* usuario);

  /**
   * Termina los hilos de ejecución creados y cierra el socket
   * del servidor.
   *
   */
  void termina_ejecucion();
  
  
  /*
  * Destructor de la clase. 
  */
  ~Servidor();

};

#endif
