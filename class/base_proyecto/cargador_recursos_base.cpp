#include "cargador_recursos_base.h"

Cargador_recursos_base::Cargador_recursos_base(DLibV::Pantalla& p):pantalla(p) 
{

}

Cargador_recursos_base::~Cargador_recursos_base() 
{

}

void Cargador_recursos_base::procesar(std::ifstream& archivo, void (Cargador_recursos_base::*procesar_valores)(const std::vector<std::string>&))
{
	const char separador='\t';

	while(!archivo.eof())
	{
		std::string linea=DLibH::Herramientas::obtener_siguiente_linea_archivo(archivo); //Leer línea en cadena.				

		if(linea.size())
		{
			std::vector<std::string> valores=DLibH::Herramientas::explotar(linea, separador);
			(this->*procesar_valores)(valores);
		}
	}	
}

void Cargador_recursos_base::generar_recursos_texturas()
{
	std::ifstream archivo(obtener_ruta_texturas().c_str(), std::ios::in);

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de texturas"<<std::endl;
	}
	else
	{
		DLibV::Gestor_texturas::vaciar();
		procesar(archivo, &Cargador_recursos_base::procesar_entrada_textura);
		archivo.close();
	}
}

void Cargador_recursos_base::generar_recursos_superficies()
{
	std::ifstream archivo(obtener_ruta_superficies().c_str(), std::ios::in);

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de superficies"<<std::endl;
	}
	else
	{
		DLibV::Gestor_superficies::vaciar();
		procesar(archivo, &Cargador_recursos_base::procesar_entrada_superficie);
		archivo.close();
	}
}

void Cargador_recursos_base::generar_recursos_audio()
{
	std::ifstream archivo(obtener_ruta_audio().c_str(), std::ios::in);

	if(!archivo)
	{
		LOG<<"ERROR: No se ha localizado el archivo de recursos_audio"<<std::endl;
	}
	else
	{
		//OJO: Vacia sonido y música.
		DLibA::Gestor_recursos_audio::liberar();
		procesar(archivo, &Cargador_recursos_base::procesar_entrada_audio);
		archivo.close();
	}
}

void Cargador_recursos_base::procesar_entrada_textura(const std::vector<std::string>& valores)
{
	if(valores.size()!=6) LOG<<"ERROR: No hay 6 parametros para recursos textura, en su lugar "<<valores.size()<<std::endl;
	else 
	{
		unsigned int indice=std::atoi(valores[0].c_str());
		std::string ruta=valores[1];
		unsigned int transparencia=std::atoi(valores[2].c_str());

		SDL_Surface * superficie=DLibV::Utilidades_graficas_SDL::cargar_imagen(ruta.c_str(), pantalla.acc_ventana());

		if(!superficie)
		{
			LOG<<"ERROR: Cargador recursos base no se ha podido cargar superficie para textura en "<<ruta<<std::endl;
		}
		else
		{
			if(transparencia)
			{
				unsigned int r=std::atoi(valores[3].c_str());
				unsigned int g=std::atoi(valores[4].c_str());
				unsigned int b=std::atoi(valores[5].c_str());

				SDL_SetColorKey(superficie, SDL_TRUE, SDL_MapRGB(superficie->format, r, g, b));
			}
			
			DLibV::Textura * t=new DLibV::Textura(pantalla.acc_renderer(), superficie);

			if(DLibV::Gestor_texturas::insertar(indice, t)==-1)
			{
				LOG<<"ERROR: No se ha podido insertar textura "<<indice<<" en "<<ruta<<std::endl;
			}	
		}
	}			
}

void Cargador_recursos_base::procesar_entrada_superficie(const std::vector<std::string>& valores)
{
	if(valores.size()!=6) LOG<<"ERROR: No hay 6 parametros para recursos superficie, en su lugar "<<valores.size()<<std::endl;
	else 
	{
		unsigned int indice=std::atoi(valores[0].c_str());
		std::string ruta=valores[1];
		unsigned int transparencia=std::atoi(valores[2].c_str());

		SDL_Surface * superficie=DLibV::Utilidades_graficas_SDL::cargar_imagen(ruta.c_str(), pantalla.acc_ventana());

		if(!superficie)
		{
			LOG<<"ERROR: Cargador recursos base no se ha podido cargar superficie para textura en "<<ruta<<std::endl;
		}
		else
		{
			if(transparencia)
			{
				unsigned int r=std::atoi(valores[3].c_str());
				unsigned int g=std::atoi(valores[4].c_str());
				unsigned int b=std::atoi(valores[5].c_str());

				SDL_SetColorKey(superficie, SDL_TRUE, SDL_MapRGB(superficie->format, r, g, b));
			}
			

			DLibV::Imagen * t=new DLibV::Imagen(superficie);

			if(DLibV::Gestor_superficies::insertar(indice, t)==-1)
			{
				LOG<<"ERROR: No se ha podido insertar superficie "<<indice<<" en "<<ruta<<std::endl;
			}	
		}
	}			
}


void Cargador_recursos_base::procesar_entrada_audio(const std::vector<std::string>& valores)
{
	if(valores.size()!=2) LOG<<"ERROR: No hay 2 parametros para recursos audio, en su lugar "<<valores.size()<<std::endl;
	else 
	{
		unsigned int indice=std::atoi(valores[0].c_str());
		std::string ruta=valores[1];

		if(DLibA::Gestor_recursos_audio::insertar_sonido(indice, ruta.c_str())==-1)
		{
			LOG<<"ERROR: No se ha podido insertar recurso audio "<<indice<<" en "<<ruta<<std::endl;
		}	
	}			
}
