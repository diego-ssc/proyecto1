#ifndef VISTA_CLIENTE_H
#define VISTA_CLIENTE_H

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib-object.h>
#include <stdio.h>
#include "../../controlador/inc/Chat.h"
#include <list>
class Cliente;

class Vista_Cliente {
  /** La ventana de bienvenida del cliente */
  GtkWidget *welcome;
  /** La ventana principal del cliente */
  GtkWidget *window;
  /** Administador de xml generado para la interfaz */
  GtkBuilder *builder;
  /** Variable para errores ocurridos al inicializar la interfaz */
  GError * err = NULL;
  /** Objeto de registro de ip */
  GtkEntry *ip;
  /** Objeto de registro de puerto */
  GtkEntry *port;
  /** Objeto de registro de mensajes */
  GtkEntry *message_box;
  /** Objeto de registro de nombre de sala */
  GtkEntry *room_creation_entry;
  /** Objeto de elección de nombre de sala */
  GtkEntry *room_election_entry;
  /** Objeto de registro de nombre de usuario */
  GtkEntry *name_entry;
  /** Botón de envío de información para la ventana de bienvenida */
  GtkButton *welcome_button;
  /** La lista de usuarios mostrada en cada sala */
  GtkWidget *user_list;		  
  /** El contenedor de la lista de usuarios mostrada en cada sala */
  GtkWidget *user_list_container;
  /** Ventana de creación de sala */
  GtkWidget *room_dialog;
  /** Ventana de elección de sala */
  GtkWidget *room_election_dialog;
  /** Ventana de creación de sala */
  GtkWidget *name_dialog;
  /** Objeto de administración de cuartos */
  GtkStack *rooms; 
  /** IP registrada por el usuario */
  const char* ip_data;
  /** Puerto registrada por el usuario */
  const char* port_data;
  /** Nombre del usuario */
  std::string username;
  /** Única instancia de la interfaz del cliente*/
  static Vista_Cliente* vista_cliente;
  /** El widget de registro de mensajes */
  GtkWidget *text_box;
  /** El widget de registro de mensajes por cuarto */
  GtkWidget *room_text_box;
  /** Lista de objetos que representan los cuartos
      de la interfaz */
  std::list<GtkWidget*> room_list;
  /** Confirma los usuarios seleccionados en
      la invitación a una sala */
  GtkButton* confirm_room_users;
  /** El nombre del usuario seleccionado para ser invitado
      a un cuarto */
  std::string selected_user;
  /** Ventana de invitación a sala */
  GtkWidget* invitation_dialog;
  /** Etiqueta de ventana de invitación */
  GtkLabel* invitation_label;
  /** El cuarto de la invitación */
  std::string invitation_roomname;
  
  /**
   * Constructor de la clase Vista_Cliente.
   * @param argc El número de argumentos pasados a la aplicación
   * @param argv Los argumentos pasados a la aplicación
   *
   */
  Vista_Cliente(int argc, char ** argv);

  /**
   * Método auxiliar que define la ventanas de error para la
   * interfaz del cliente.
   * @param error_message El mensaje de error.
   * @param parent La ventana padre.
   *
   */
  void window_error(const char * error_message, GtkWidget* parent);

  /**
   * Método auxiliar que define la ventanas de advertencia para la
   * interfaz del cliente.
   * @param error_message El mensaje de advertencia.
   * @param parent La ventana padre.
   *
   */
  void window_warning(const char * warning_message, GtkWidget* parent);

  /**
   * Método auxiliar que define la ventanas de información para la
   * interfaz del cliente.
   * @param error_message El mensaje de información.
   * @param parent La ventana padre.
   *
   */
  void window_info(const char * info_message, GtkWidget* parent);
  
public:

  /**
   * Elimina el constructor copia de la clase.
   *
   */
  Vista_Cliente(Vista_Cliente &other) = delete;

  /**
   * Elimina el operador de asignación.
   *
   */
  void operator=(const Vista_Cliente &) = delete;

  /**
   * Devuelve la única instancia de la vista del cliente.
   * @param argc El número de argumentos pasados a la aplicación
   * @param argv Los argumentos pasados a la aplicación
   * @return La única instancia de la vista del cliente
   *
   */
  static Vista_Cliente *get_instance(int argc, char **argv) {
    if (vista_cliente == nullptr) {
      vista_cliente = new Vista_Cliente(argc, argv);
    }
  
    return vista_cliente;
  }

  /**
   * Devuelve la única instancia de la vista del cliente.
   * @return La única instancia de la vista del cliente
   *
   */
  static Vista_Cliente *get_instance() {    
    return vista_cliente;
  }
  
  /**
   * Define el ip del cliente.
   * @param ip_data El IP al que se conectará el cliente
   *
   */
  void set_ip_data(const char* ip_data);

  /**
   * Define el puerto del cliente.
   * @param port_data El puerto al que se conectará el cliente
   *
   */
  void set_port_data(const char* port_data);

  /**
   * Define el nombre del usuario conectado.
   * @param username El nombre del usuario 
   *
   */  
  void set_username(std::string username);

  /**
   * Define el nombre del usuario que fue invitado a una
   * sala.
   * @param EL nombre del usuario.
   *
   */  
  std::string set_selected_user(std::string username);
  
  /**
   * Devuelve el ip del cliente.
   * @return El IP del cliente
   *
   */
  const char* get_ip_data();

  /**
   * Devuelve el puerto del cliente.
   * @return El puerto del cliente
   *
   */
  const char* get_port_data();

  /**
   * Devuelve el nombre del usuario registrado.
   * @return El nombre de usuario registrado
   *
   */ 
  std::string get_username();
  
  /**
   * Devuelve el objeto que registró el IP del cliente.
   * @return El objeto que registró el IP del cliente
   *
   */
  GtkEntry* get_ip_entry();

  /**
   * Devuelve el objeto que registró el puerto del cliente.
   * @return El objeto que registró el puerto del cliente
   *
   */
  GtkEntry* get_port_entry();

  /**
   * Devuelve el objeto que registró el mensaje del cliente.
   * @return El objeto que registró el mensaje del cliente
   *
   */
  GtkEntry* get_message_entry();

  /**
   * Devuelve el objeto que guarda el nombre de la sala a crear.
   * @return El objeto que registró el nombre de la sala
   * a crear
   *
   */
  GtkEntry* get_room_creation_entry();

  /**
   * Devuelve el objeto que guarda el nombre de la sala a elegir.
   * @return El objeto que registró el nombre de la sala
   * a elegir
   *
   */
  GtkEntry* get_room_election_entry();

  /**
   * Devuelve el objeto que guarda el nombre usuario.
   * @return El objeto que registró el nombre de usuario.
   *
   */
  GtkEntry* get_name_entry();

  /**
   * Devuelve el objeto que registró la lista de usuarios.
   * @return El objeto que registró la lista de usuarios
   *
   */
  GtkWidget* get_user_list();
  
  /**
   * Devuelve el objeto contenedor de la lista de usuarios.
   * @return El objeto que contenedor de la lista de usuarios
   *
   */
  GtkWidget* get_user_list_container();
  
  /**
   * Devuelve el widget de la ventana de bienvenida.
   * @return El widget de la ventana de bienvenida
   *
   */
  GtkWidget * get_welcome_widget();

  /**
   * Devuelve el widget de la ventana del cliente.
   * @return El widget de la ventana del cliente
   *
   */
  GtkWidget * get_client_widget();

  /**
   * Devuelve el widget de la ventana de creación
   * de salas.
   * @return El widget de la ventana de creación
   * de salas
   *
   */
  GtkWidget * get_room_dialog();

  /**
   * Devuelve el widget de la ventana de elección
   * de salas.
   * @return El widget de la ventana de elección
   * de salas
   *
   */
  GtkWidget * get_room_election_dialog();

  /**
   * Devuelve el widget de la ventana de registro
   * de nombre de usuario.
   * @return El widget de la ventana de registro
   * de nombre de usuario
   *
   */
  GtkWidget * get_name_dialog();

  /**
   * Devuelve el widget de registro de mensajes.
   * @return El widget de registro de mensajes
   *
   */
  GtkWidget* get_text_box();
  
  /**
   * Devuelve el widget de registro de mensajes
   * de un cuarto.
   * @return El widget de registro de mensajes
   * de un cuarto.
   *
   */
  GtkWidget* get_room_text_box();

  /**
   * Devuelve el objeto de administración de
   * cuartos.
   * @return El objeto de administración de
   * de cuartos.
   *
   */
  GtkStack* get_rooms_stack();

  /**
   * Devuelve la lista de los cuartos del
   * cliente.
   * @return La lista de los cuartos del
   * cliente.
   *
   */
  std::list<GtkWidget*> get_room_list();

  /**
   * Devuelve el botón que confirma los usuarios seleccionados
   * para la invitación a un cuarto.
   * @return El botón que confirma los usuarios seleccionados
   * para la invitación a un cuarto.
   *
   */
  GtkButton* get_confirm_room_users();

  /**
   * Devuelve el nombre del usuario que fue invitado a una
   * sala.
   * @return EL nombre del usuario.
   *
   */  
  std::string get_selected_user();
  
  /**
   * Devuelve la ventana de invitación a sala
   * @return La ventana de invitación a sala
   *
   */
  GtkWidget* get_invitation_dialog();

  /**
   * Devuelve la etiqueta de invitación a sala
   * @return La etiqueta de invitación a sala
   *
   */
  GtkLabel* get_invitation_label();

  /**
   * Devuelve el nombre del cuarto de la invitación
   * @return El nombre del cuarto de la invitación
   *
   */
  std::string get_invitation_roomname();
  
  /**
   * Muestra la ventana de bienvenida del cliente.
   *
   */
  void welcome_window();

  /**
   * Muestra la ventana de introducción del nombre
   * del usuario.
   *
   */
  void client_name_window();
  
  /**
   * Muestra la ventana principal del cliente.
   *
   */
  void client_window(Cliente* cliente);

  /**
   * Inicia las variables asociadas a UI
   * de los cuartos del cliente.
   *
   */
  void rooms_widget(const gchar* roomname);

  /**
   * Muestra una ventana de diálogo en caso de
   * que el puerto registrado no sea válido.
   *
   */
  void window_port_error();

  /**		      
   * Muestra una ventana de diálogo en caso de
   * que el IP registrado no sea válido.
   *
   */
  void window_ip_error();

  /**		      
   * Muestra una ventana de diálogo en caso de
   * que el host registrado no exista.
   *
   */
  void window_null_server_error();

  /**		      
   * Muestra una ventana de diálogo en caso de
   * que el socket no haya podido abrirse.
   *
   */
  void window_socket_error();

  /**		      
   * Muestra una ventana de diálogo en caso de
   * que el cliente no haya podido establecer
   * una conexión con el servidor.
   *
   */
  void window_connection_error();

  /**		      
   * Muestra una ventana de diálogo en caso de
   * que no se haya podido escribir los datos
   * al socket.
   *
   */
  void window_write_error();

  /**		      
   * Muestra una ventana de diálogo en caso de
   * que no se haya podido leer los datos
   * del socket anfitrión.
   *
   */
  void window_read_error();
  
  /**
   * Método auxiliar que define la ventana de introducción del
   * nombre de la sala que se desea crear.
   *
   */
  void window_room_creation();

  /**
   * Método auxiliar que define la ventana de introducción del
   * nombre de la sala que a la que se desea invitar usuarios.
   *
   */
  void window_room_election(char* name);

  /**
   * Método que define la ventana de invitación a un cuarto.
   * @param message El mensaje de invitación.
   * @param roomname El nombre del cuarto.
   *
   */
  void window_invitation(std::string message,
			 std::string roomname);

  /**
   * Muestra una ventana de diálogo asociada a la ventana
   * principal del cliente en caso de surgir un mensaje de
   * error.
   * @param message El mensaje asociado al error.
   *
   */
  void window_main_error(std::string message);

  /**
   * Muestra una ventana de diálogo asociada a la ventana
   * principal del cliente en caso surgir un mensaje de
   * advertencia.
   * @param message El mensaje asociado a la advertencia.
   *
   */
  void window_main_warning(std::string message);

  /**
   * Muestra una ventana de diálogo asociada a la ventana
   * principal del cliente en caso de surgir un mensaje de
   * información.
   * @param message El mensaje asociado a la información
   * provista.
   *
   */
  void window_main_info(std::string message);

};

#endif

