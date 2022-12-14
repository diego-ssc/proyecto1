#pragma GCC diagnostic ignored "-Wswitch" // No se usarán todos miembros del enum Protocolo
#include "inc/Servidor.h"

#define MAX_LEN 1024

Servidor::Servidor(int puerto) {
  bzero((char *) &this->direccion_servidor, sizeof(this->direccion_servidor));
  this->direccion_servidor.sin_family = AF_INET;
  this->direccion_servidor.sin_addr.s_addr = INADDR_ANY;//inet_addr(ip);
  this->direccion_servidor.sin_port = htons(puerto);
  this->puerto = puerto;
  this->procesador = Procesador_Servidor();
}

char* Servidor::get_host_buffer() {
  return this->host_buffer;
}

int Servidor::get_file_descriptor() {
  return this->file_descriptor;
}

int Servidor::get_seed() {
  return this->seed;
}

int Servidor::get_read_status() {
  return this->read_status;
}
 
int Servidor::get_write_status() {
  return this->write_status;
}

void Servidor::set_seed() {
  this->seed++;
}

void Servidor::set_read_status(int status) {
  this->read_status = status;
}

void Servidor::set_write_status(int status) {
  this->write_status = status;
}

void Servidor::set_text_buffer(GtkTextBuffer* text_buffer) {
  this->text_buffer = text_buffer;
}

int Servidor::servidor_socket() {
  int socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  this->file_descriptor = socket_file_descriptor;
  return socket_file_descriptor;
}

int Servidor::servidor_bind() {
  int status = bind(this->file_descriptor,
		    (struct sockaddr *) &this->direccion_servidor,
		    sizeof(this->direccion_servidor));

  return status;
}

int Servidor::servidor_listen(int tamano_cola) {
  int status = listen(this->file_descriptor, tamano_cola);

  return status;
}

int Servidor::servidor_accept() {
  socklen_t longitud_cliente = sizeof(this->direccion_cliente);
  std::cout<<"[Servidor] Aceptando..."<<std::endl;
  int socket = accept(this->file_descriptor
                      , (struct sockaddr *) &this->direccion_cliente
                      , &longitud_cliente);
  
  return socket;
}

int Servidor::servidor_send(int descriptor_archivo, char * message) {
  // sizeof(message) es menor a 1024
  // si es usado en send, mandará un mensaje cortado
  return send(descriptor_archivo, message, 1024, 0);
}

int Servidor::servidor_read(int descriptor_archivo) {
  const int tamano_buffer = 1024;
  bzero(this->host_buffer, tamano_buffer);// Limpia el buffer, en caso de no estar vacío 
  int status = read(descriptor_archivo, this->host_buffer, tamano_buffer);

  return status;
}

void Servidor::servidor_close(int descriptor_archivo) {
  close(descriptor_archivo);
}

void Servidor::global_message(std::string message) {
  char temp[MAX_LEN];
  strcpy(temp, message.c_str());
  std::map<std::string, Usuario*>::iterator i;
  for (i = usuarios.begin(); i != usuarios.end(); ++i) {
    if(servidor_send(i->second->get_socket(), temp))
      write_status = -1;
  }
}

void Servidor::global_message_from(std::string message,
				   int sender_id) {
  char temp[MAX_LEN];
  strcpy(temp, message.c_str());
  std::map<std::string, Usuario*>::iterator i;
  printf("Mensaje devuelto: %s", temp);
  for (i = usuarios.begin(); i != usuarios.end(); ++i) {
    std::cout<<"Nombre usuario: "<<i->second->get_name()<<std::endl;
    std::cout<<"Socket: "<<i->second->get_socket()<<std::endl;
    std::cout<<"ID: "<<i->second->get_id()<<std::endl<<std::endl<<std::endl;
    std::cout<<"Sender id: "<<sender_id<<std::endl;
    if(i->second->get_id() == sender_id) continue;
    if(servidor_send(i->second->get_socket(), temp) < 0)
      write_status = -1;
  }
}

void Servidor::send_message_to(std::string message, int socket) {
  char temp[MAX_LEN];
  std::cout<<"Socket: "<<socket<<std::endl;
  strcpy(temp, message.c_str());
  if(servidor_send(socket, temp) < 0)
    write_status = -1;
}

void Servidor::administra_cliente(Usuario* usuario) {
  int client_socket;
  int id;
  std::string name;
  
  while (true) {
    client_socket = usuario->get_socket();
    id = usuario-> get_id();
    if (servidor_read(client_socket) == -1)
      read_status = -1;
    
    Procesador_Servidor procesador;
    std::list<std::string>(Procesador::*procesador_parse)(std::string) =
      Fabrica_Procesadores::recv_protocol(Protocolo::IDENTIFY);
    std::list<std::string> name_list = (procesador.*procesador_parse)
      (std::string(get_host_buffer()));
    
    usuario->set_name(name_list.front());
    name = usuario->get_name();
    std::map<std::string, Usuario*>::iterator i;
    i = usuarios.find(name);
    
    if (i != usuarios.end()) {
      std::string id_failure = procesador.write_message_id_failure(name);
      send_message_to(id_failure, usuario->get_socket());
      continue;
    }
    break;
  }
  
  client_mutex.lock();
  usuarios[name] = std::move(usuario);
  client_mutex.unlock();
  GtkTextIter iter;
  gtk_text_buffer_get_iter_at_offset(text_buffer, &iter, 0);
  gtk_text_buffer_create_tag(text_buffer, "lmarg", 
			     "left_margin", 5, NULL);
  gtk_text_buffer_insert_with_tags_by_name(text_buffer, &iter, 
					   (name + " se ha conectado\n").c_str(),
					   -1, NULL, "lmarg",  NULL);
  
  std::string(Procesador::*procesador_write_one)(std::string) = 
    Fabrica_Procesadores::send_protocol_one(Protocolo::IDENTIFY);

  send_message_to(procesador.write_message_id_success(), client_socket); 
    
  std::string new_user_global = (procesador.*procesador_write_one)(name);
  global_message_from(new_user_global, id);


  // Variables para usarse dentro de while
  std::string message;
  std::string username;
  std::string roomname;
  std::string(Procesador::*write_one_arguments)(std::string);
  std::string(Procesador::*write_two_arguments)(std::string, std::string);
  std::list<std::string> lista;
  std::map<std::string, Usuario*>::iterator i;
  std::map<std::string, Cuarto*>::iterator j;
  std::list<std::string>::iterator k;
  Cuarto* cuarto;
  int status;
  
  while(true) {
    if (servidor_read(client_socket) <= 0)
      return;

    Protocolo protocolo = procesador.get_type(get_host_buffer());
    std::list<std::string>(Procesador::*parse)(std::string) =
      Fabrica_Procesadores::recv_protocol(protocolo);
    std::list<std::string> recvd_message = (procesador.*parse)
      (std::string(get_host_buffer()));
    
    write_one_arguments = Fabrica_Procesadores::send_protocol_one(protocolo);
    write_two_arguments = Fabrica_Procesadores::send_protocol_two(protocolo);

    switch (protocolo) {
    case STATUS:
      if (strcmp(usuario->get_status().c_str(), recvd_message.front().c_str()) == 0) {
	send_message_to(procesador.write_message_status_failure(usuario->get_status()),
			client_socket);
	break;
      }

      usuario->set_status(recvd_message.front());
      send_message_to(procesador.write_message_status_success(),
		      client_socket);
      message = (procesador.*write_two_arguments)(name, recvd_message.back());
      global_message_from(message, id);
      break;
    case USERS:
      for (i = usuarios.begin(); i != usuarios.end(); ++i) 
	lista.push_back(i->first);
      
      send_message_to(procesador.write_message_users(lista), client_socket);
      lista.clear();
      break;
    case MESSAGE:
      message = (procesador.*write_two_arguments)(name, recvd_message.back());
      username = recvd_message.front();
      i = usuarios.find(username);
      if (i != usuarios.end()) 
	send_message_to(message, i->second->get_socket());
      else
	send_message_to(procesador.write_message_private_message_failure(username),
			client_socket);
      break;
    case PUBLIC_MESSAGE:
      message = (procesador.*write_two_arguments)(name, recvd_message.back());
      global_message_from(message, id);
      break;
    case NEW_ROOM:
      roomname = recvd_message.front();
	j = cuartos.find(roomname);
      if (j == cuartos.end()) {
	procesador.write_message_new_room_success();
	cuarto = new Cuarto(roomname);
	cuartos[roomname] = std::move(cuarto);
	cuarto->first_member(name, usuario);
      }	else {
	send_message_to(procesador.write_message_new_room_failure(roomname),
			client_socket);
      }

     
      break;
    case INVITE:
      roomname = recvd_message.front();
      j = cuartos.find(roomname);
      if (j == cuartos.end()) {
	send_message_to(procesador.write_message_invite_failure_room(roomname),
			client_socket);
	break;
      }
      
      cuarto = cuartos.at(roomname);
      for (k = recvd_message.begin(); k != recvd_message.end(); ++k) {
	if (k == recvd_message.begin())
	  continue;
	i = usuarios.find(*k);
	if (i == usuarios.end()) {
	  send_message_to(procesador.write_message_invite_failure_users(*k),
			  client_socket);
	  break;
	}
	
	Usuario *user = std::move(usuarios.at(*k));
	cuarto->invite_member(user->get_name(), user);
	message = (procesador.*write_two_arguments)(roomname, name);
	send_message_to(message, user->get_socket());
      }
      send_message_to(procesador.write_message_invite_success(roomname),
		      client_socket);
      break;
      
    case JOIN_ROOM:
      roomname = recvd_message.front();
      j = cuartos.find(roomname);
      if (j == cuartos.end()) {
	send_message_to(procesador.write_message_join_room_failure_existence(roomname),
			client_socket);
	break;
      }

      cuarto = cuartos.at(roomname);
      status = cuarto->add_member(usuario->get_name(), usuario); 
      if (status == -1) {
	send_message_to(procesador.write_message_join_room_failure_joined(roomname),
			client_socket);
	break;
      } else if (status == -2) {
	send_message_to(procesador.write_message_join_room_failure_invite(roomname),
			client_socket);
	break;
      }
      send_message_to(procesador.write_message_join_room_success(roomname),
		      client_socket);
      lista = cuarto->get_room_usernames();
      for (k = lista.begin(); k != lista.end(); ++k) {
	if ((*k).compare(usuario->get_name()) == 0) continue;
	send_message_to((procesador.*write_two_arguments)(roomname, name),
			usuarios.at(*k)->get_socket());
      }
      break;
      
    case ROOM_USERS:
      roomname = recvd_message.front();
      j = cuartos.find(roomname);
      if (j == cuartos.end()) {
	send_message_to(procesador.write_message_room_users_failure_existence(roomname),
			client_socket);
	break;
      }
      cuarto = cuartos.at(roomname);
      if (!cuarto->is_invited(name)
	  || (cuarto->is_invited(name)
	      && !cuarto->is_in(name))) {
	send_message_to(procesador.write_message_room_users_failure_invite(roomname),
			client_socket);
	break;
      }

      send_message_to(procesador.write_message_room_users_new
		      (cuarto->get_room_usernames()),
		      client_socket);
      break;
	
    case ROOM_MESSAGE:
      roomname = recvd_message.front();
      message = recvd_message.back();
      j = cuartos.find(roomname);
      if (j == cuartos.end()) {
	send_message_to(procesador.write_message_room_message_failure_existence(roomname),
			client_socket);
	break;
      }
      cuarto = cuartos.at(roomname);
      if ((!cuarto->is_invited(name) &&
	   !cuarto->is_in(name))
	  || (cuarto->is_invited(name)
	      && !cuarto->is_in(name))) {
	send_message_to(procesador.write_message_room_message_failure_invite(roomname),
			client_socket);
	break;
      }
      lista = cuarto->get_room_usernames();
      for (k = lista.begin(); k != lista.end(); ++k) {
	if ((*k).compare(name) == 0) continue;
	send_message_to(procesador.write_message_room_message_new(roomname,
								  name,
								  message),
			usuarios.at(*k)->get_socket());
      }
      break;
      
    case LEAVE_ROOM:
      roomname = recvd_message.front();
      j = cuartos.find(roomname);
      if (j == cuartos.end()) {
	send_message_to(procesador.write_message_leave_room_failure_existence(roomname),
			client_socket);
	break;
      }
      cuarto = cuartos.at(roomname);
      if (!cuarto->is_invited(name)
	  || (cuarto->is_invited(name)
	      && !cuarto->is_in(name))) {
	send_message_to(procesador.write_message_leave_room_failure_invite(roomname),
			client_socket);
	break;
      }
      send_message_to(procesador.write_message_leave_room_success(roomname),
		      client_socket);
      lista = cuarto->get_room_usernames();
      for (k = lista.begin(); k != lista.end(); ++k) {
	send_message_to(procesador.write_message_room_message_new(roomname,
								  name,
								  message),
			usuarios.at(*k)->get_socket());
      }
      break;
	
    case DISCONNECT:
      message = (procesador.*write_one_arguments)(name);
      for (j = cuartos.begin(); j != cuartos.end(); ++j) {
	if (j->second->eliminate_member(name) == -1)
	  continue;
	lista = j->second->get_room_usernames();
	for (k = lista.begin(); k != lista.end(); ++k) {
	  send_message_to(procesador.write_message_disconnect_new_users(name),
			  usuarios.at(*k)->get_socket());
	}
	
      }
      
      for (i = usuarios.begin(); i != usuarios.end(); ++i) {
	send_message_to(message,
			i->second->get_socket());
      }
      break;
    }
  } 
}

std::thread Servidor::crea_hilo(Usuario* usuario) {
  std::thread t(&Servidor::administra_cliente, this, usuario);
  return t;
}

void Servidor::termina_ejecucion() {
  if (usuarios.empty()) return;
  Usuario usuario;
  std::map<std::string, Usuario*>::iterator i;
  for (i = usuarios.begin(); i != usuarios.end(); ++i) {
    std::thread t = i->second->get_thread();
    delete i->second;
    t.detach();
  }
  close(file_descriptor);
}

Servidor::~Servidor() {}
