#ifdef __INTERNE_ABAQUS
/*****************************************************************************\
 *    L I M A + + : Bibliotheque de lecture / ecriture de maillages 2D ou 3D *
\*****************************************************************************/


/******************************************************************************
 Fichier : reader_abaqus.C
 Cree le : 
 Derniere modification le : 
 Auteur  : S. Chemin
******************************************************************************/

/*!   
  \file reader_abaqus.C
  \brief Interface de lecture / ecriture pour les fichiers Abaqus. 
*/

#include "LimaP/myio.h"
#include "LimaP/misc.h"

#include <ctype.h>
#include <errno.h>

#include "LimaP/maillage_it.h"
#include "LimaP/algorithme.h"
#include "LimaP/reader_abaqus.h" 
#include "LimaP/MutableString.h"
#include "LimaP/using_it.h"


BEGIN_NAMESPACE_LIMA


size_type _ReaderAbaqus::lima_faces[6][6] =
  {
    {0, 2, 3, 1, 5, 5}, // TETRAEDRE
    {0, 0, 0, 0, 0, 0}, // PYRAMIDE
    {0, 3, 2, 4, 1, 6}, // PRISME
    {0, 3, 2, 4, 5, 1},  // HEXAEDRE
    {0, 1, 2, 4, 4, 4}, // TRIANGLE
    {0, 1, 2, 3, 5, 5} // QUADRANGLE
  };


pair<string,int> _ReaderAbaqus::keyword_pairs[] =
  {
    // Mots clef traités
    pair<string,int>("NODE",ABAQUS_NODE),
    pair<string,int>("ELEMENT",ABAQUS_ELEMENT),
    pair<string,int>("NSET",ABAQUS_NSET),
    pair<string,int>("ELSET",ABAQUS_ELSET),
    pair<string,int>("SURFACE",ABAQUS_SURFACE),
    pair<string,int>("INSTANCE",ABAQUS_INSTANCE),
    pair<string,int>("PART",ABAQUS_PART),   
    pair<string,int>("SYSTEM",ABAQUS_SYSTEM),

    // Mots clefs non traités => Arrêt de l'e×écution
    pair<string,int>("NGEN",ABAQUS_NGEN),
    pair<string,int>("NCOPY",ABAQUS_NCOPY),
    pair<string,int>("ELGEN",ABAQUS_ELGEN),
    pair<string,int>("ELCOPY",ABAQUS_ELCOPY),
    pair<string,int>("NFILL",ABAQUS_NFILL),
    pair<string,int>("ORIENTATION",ABAQUS_ORIENTATION),
    pair<string,int>("INCLUDE",ABAQUS_INCLUDE),
    pair<string,int>("PARAMETER",ABAQUS_PARAMETER),
    pair<string,int>("PARAMETERS",ABAQUS_PARAMETERS),
    pair<string,int>("IMPORT",ABAQUS_IMPORT),

    pair<string,int>("END",ABAQUS_END)
  };
  
// Correspondances des types
// On ignore les formulations : ac3d8 est valable pour ac3d8, ac3d8r, ac3d8i etc...
pair<string,int> _ReaderAbaqus::type_pairs[]=
  {
    //BRAS
    pair<string,int>("AC1D2", BRAS),
    pair<string,int>("ASI2D2", BRAS),
    pair<string,int>("B21", BRAS),
    pair<string,int>("B31", BRAS),
    pair<string,int>("DC1D2", BRAS),
    pair<string,int>("DCC1D2", BRAS),
    pair<string,int>("ELBOW31", BRAS),
    pair<string,int>("F2D2", BRAS),
    pair<string,int>("FLINK", BRAS),
    pair<string,int>("FRAME2D", BRAS),
    pair<string,int>("FRAME3D", BRAS),
    pair<string,int>("GK2D2", BRAS),
    pair<string,int>("GK3D3", BRAS),
    pair<string,int>("PIPE21", BRAS),
    pair<string,int>("PIPE31", BRAS),
    pair<string,int>("R2D2", BRAS),
    pair<string,int>("RB2D2", BRAS),
    pair<string,int>("RB3D2", BRAS),
    pair<string,int>("T2D2", BRAS),
    pair<string,int>("T3D2", BRAS), 

    // TRIANGLES
    pair<string,int>("AC2D3",POLYGONE),
    pair<string,int>("ASI3D3",POLYGONE),
    pair<string,int>("CPE3",POLYGONE),
    pair<string,int>("CPEG3",POLYGONE),
    pair<string,int>("CPD3",POLYGONE),
    pair<string,int>("DC2D3",POLYGONE),
    pair<string,int>("DS3",POLYGONE),
    pair<string,int>("F3D3",POLYGONE),
    pair<string,int>("M3D3",POLYGONE),
    pair<string,int>("R3D3",POLYGONE),
    pair<string,int>("S3",POLYGONE),
    pair<string,int>("SFM3D3",POLYGONE),
    pair<string,int>("STRI3",POLYGONE),
    pair<string,int>("WARP2D3",POLYGONE),

    // QUADRANGLES
    pair<string,int>("AC2D4",POLYGONE),
    pair<string,int>("ASI3D4",POLYGONE),
    pair<string,int>("COH2D4",POLYGONE),
    pair<string,int>("CPE4",POLYGONE),
    pair<string,int>("CPEG4",POLYGONE),
    pair<string,int>("CPS4",POLYGONE),
    pair<string,int>("CAX4",POLYGONE),	// v 6.33.0, 2D plan
    pair<string,int>("CAX4R",POLYGONE),	// v 6.33.0, 2D axi
    pair<string,int>("DC2D4",POLYGONE),
    pair<string,int>("DCC2D4",POLYGONE),
    pair<string,int>("F3D4",POLYGONE),
    pair<string,int>("GK3D4",POLYGONE),
    pair<string,int>("GKPE4",POLYGONE),
    pair<string,int>("GKPS4",POLYGONE),
    pair<string,int>("M4D4",POLYGONE),
    pair<string,int>("R3D4",POLYGONE),
    pair<string,int>("S4",POLYGONE),
    pair<string,int>("S4R5",POLYGONE),
    pair<string,int>("SFM3D4",POLYGONE),
    pair<string,int>("WARP2D4",POLYGONE),
    //TETRAEDRES
    pair<string,int>("AC3D4",POLYEDRE),
    pair<string,int>("C3D4",POLYEDRE),
    pair<string,int>("DC3D4",POLYEDRE),

    //PRISMES
    pair<string,int>("AC3D6",POLYEDRE),
    pair<string,int>("C3D6",POLYEDRE),
    pair<string,int>("DC3D6",POLYEDRE),
    pair<string,int>("GK3D6",POLYEDRE),
    pair<string,int>("COH3D6",POLYEDRE),
    pair<string,int>("SC6",POLYEDRE), // 6

    //HEXAEDRES
    pair<string,int>("AC3D8",POLYEDRE),
    pair<string,int>("C3D8",POLYEDRE),
    pair<string,int>("COH3D8",POLYEDRE),
    pair<string,int>("DC3D8",POLYEDRE),
    pair<string,int>("DCC3D8",POLYEDRE),
    pair<string,int>("GK3D8",POLYEDRE),
    pair<string,int>("SC8",POLYEDRE),
  }; 


map<string,int> _ReaderAbaqus::abaqus_keywords(keyword_pairs,
					       keyword_pairs+sizeof(keyword_pairs)/sizeof(pair<string,int>));

map<string,int> _ReaderAbaqus::types_elements(type_pairs,
					      type_pairs+sizeof(type_pairs)/sizeof(pair<string,int>));

 

//////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------*/
/*!
  \brief Structure permettant de convertir les mots clefs Abaqus en majuscule
*/
/*--------------------------------------------------------------------------*/
struct majuscule
{
  char operator()(char c) const
  {
    return toupper(static_cast<unsigned char>(c));
  }
};

//////////////////////////////////////////////////////////////////////////////
_ReaderAbaqus::_ReaderAbaqus(_MaillageInterne* mesh, const string& nom_fichier, format_t format) : _Reader(mesh, nom_fichier, format) ,
	n_ligne (0)
{
  
  m_filecpp.ascii = true;
  m_filecpp.open();

  nbnodes_glob=0;
  nbelems_glob=0;  

  nbnodes_inst=0;
  nbelems_inst=0;

  ifstream & fcpp=m_filecpp.fp;
  pos_init=fcpp.tellg();

  maillage_dim=0;
  dim_lu=false;
}

//////////////////////////////////////////////////////////////////////////////
_ReaderAbaqus::~_ReaderAbaqus()
{
  m_filecpp.close();
}


//////////////////////////////////////////////////////////////////////////////
void _ReaderAbaqus::readAll ( )
{
  try
    {
      m_mai->debut_lecture (m_filecpp.full_name, 0);
      lire ( );
      m_mai->fin_lecture (m_filecpp.full_name, 0);
    }
  catch (const _Reader::LectureAnnulee&)
    {
      m_mai->vider ( );
      throw read_erreur ("Lecture annulée par l'application.");
    }
	catch (const read_erreur& re)	// v 6.3.2
	{
		throw;
	}
	catch (const exception& stdexc)
	{	// v 6.3.2
		throw read_erreur (stdexc.what ( ));
	}
  catch (...)
    {
//      throw;
		throw read_erreur ("Erreur non documentée.");	// v 6.3.2
    }
}

//////////////////////////////////////////////////////////////////////////////
void _ReaderAbaqus::readStructure() 
{
  throw read_erreur ("Lecture de la structure non supportée avec le format Abaqus.");
}

//////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------*/
/*!
  \brief Verification de la fin d'un bloc (NODE, ELEMENT, ELSET,SURFACE...)
  -> retourne 1 si le debut du bloc suivant est atteint (symbole '*')      
  -> retourne 2 si une ligne de commentaire est rencontrée (symbole '**') 
  -> retourne 3 en cas de fin de fichier
  -> retourne 0 sinon
*/
/*--------------------------------------------------------------------------*/

int _ReaderAbaqus::check_bloc_end(ifstream& fp)
{
  char c0 = fp.get();
  size_t nbb=1;

	if (true == fp.eof ( ))
		return 3;	// CP v 6.27.0

  while(c0==' '){
        nbb++;
        c0 = fp.get();
  }
 
  const char c1 = c0;
  if (c1 == '*') {

    const char c2 = fp.get();
    if (c2 == '*') 
      return 2;  
      
    fp.unget();
    fp.unget();
    return 1;
  }
  fp.unget();

  return 0;
}

//////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------*/
/*!
  \brief  Fonction permettant de connaître le type d'un élément en fonction        
  du type de groupe Abaqus traité (NSET ou ELSET)                          
  - si groupe Abaqus = NSET -> type=NOEUD                                  
  - si groupe Abaqus = ELSET -> type=BRAS,POLYGONE ou POLYEDRE             
*/
/*--------------------------------------------------------------------------*/

int _ReaderAbaqus::getElemType(int id,int type_groupe_abaqus)
{
  int type;

  switch(type_groupe_abaqus) {
  case ABAQUS_NSET:
    type=NOEUD;
    break;
        
  case ABAQUS_ELSET:
    type=type_element_lima[id];
    break;

  default:
    throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : le type de groupe Abqaqus est incorrect. Les types reconnus sont NSET et ELSET.\n ");
    break;
  }
  return type;
}
//////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------*/
/*!
  \brief Fonction permettant de détecter si une chaîne est de type numérique      
  ou non
*/
/*--------------------------------------------------------------------------*/

bool _ReaderAbaqus::num_string(string& str1)
{
  bool num=false;
  int i=0;

  while(!num && i<str1.length()){
	
    num=isdigit(str1.at(i))==0;
    i++;
  }

  return !num;
}
//////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------*/
/*!
  \brief  Fonction permettant de détecter si une chaîne est vide (de taille nulle  
  ou constituée exclusivement de caractères 'espace')   
*/
/*--------------------------------------------------------------------------*/

bool _ReaderAbaqus::empty_line(string& str1)
{
  bool space=true;
  int i=0;

  while(space && i<str1.length()) {
    space=isspace(str1.at(i))>0;
    i++;
  }
    
  return space;
}
//////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------*/
/*!
  \brief  Detection et lecture d'une ligne de mots clefs Abaqus. Seules les lignes 
  qui débutent par le caractère '*' sont traitées. Les lignes vides sont ignorées.                                                  
  Si une ligne de mots clefs est détectée, on renvoi lu=true ainsi que 
  la liste des mots clefs dans le vecteur params.  
*/
/*--------------------------------------------------------------------------*/

bool _ReaderAbaqus::lire_ligne(ifstream& fp,vector<string>& params)
{
  string buffer;
  bool lu	= false;	// Correctif CP : variable non intialisée

  getline(fp,buffer);
  n_ligne++;

  if(buffer=="*") 
      return false;
   
	if(!empty_line(buffer))
	{
		while(buffer.at(0)==' ')
		{
			buffer.erase(buffer.begin());
		}     
    
		if(buffer.at(0)=='*')
		{
   
			buffer.erase(buffer.begin());
			split_string(buffer," ,=\r\n",params);
			lu=true;
		}
		else 
			lu=false;
	}  
   
  return lu;

}
//////////////////////////////////////////////////////////////////////////////

/*--------------------------------------------------------------------------*/
/*!
  \brief  Lecture des noeuds du maillage (bloc NODE) :                            
  - traitement des options passées au mot clef NODE (vecteur params).     
  - création d'un nuage si l'option NSET est active.                      
  - lecture des coordonnées des noeuds.                                   
  - ajout des noeuds au maillage et au nuage si l'option NSET est active.  
*/
/*--------------------------------------------------------------------------*/

void _ReaderAbaqus::lire_noeuds(ifstream& fp,vector<string>& params,string nom_instance_appelante)
{
 
  string line;
  string nom_groupe;
  string nom_groupe_glob="";
  string nom_instance;
  vector<double> coords;  
  bool cancel=false;
  double sy=0.; 
  double sz=0.;

  // Options entrainant le non traitement de la commande
  vector<string> ignore_options;
  ignore_options.push_back("PRINT");
  ignore_options.push_back("OUTPUT");
  ignore_options.push_back("FILE");

  // Options interdites
  vector<string> bad_options;
  bad_options.push_back("INPUT"); 
   
    /* ---------------------------------Traitement des options--------------------------------- */


    // recherche des options qui annulent le traitement de la commande
    for(int i=0;i<ignore_options.size();i++){
      vector<string>::iterator it=find(params.begin(),params.end(),ignore_options[i]);
      if(it<params.end())
	return;
    }

    // recherche des options interdites
    for(int i=0;i<bad_options.size();i++){
      vector<string>::iterator it=find(params.begin(),params.end(),bad_options[i]);
//      if(it<params.end()) 
	if (params.end ( ) != it)	// // CP, v 6.27.1, coverity
		throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : " + *it+", option de NODE non traitée \n");
    }
           
    nom_instance = nom_instance_appelante;

    // option NSET
    vector<string>::iterator itopt=find(params.begin(),params.end(),"NSET");  
    nom_instance = nom_instance_appelante;

    if(itopt<params.end()) {
      nom_groupe=*(itopt+1);
      nom_groupe_glob=nom_instance+"_"+nom_groupe+"_NSET";

      // si option NSET active -> on ajoute le nuage au maillage s'il n'existe pas encore
      if(!contient_groupe(m_mai,nom_groupe_glob,NOEUD)) {
	ajouter_groupe(NOEUD,nom_groupe_glob,m_mai);
	groupe_abaqus.insert(pair<string,string>(nom_groupe,nom_groupe_glob));
	types_groupes.insert(pair<string,int>(nom_groupe_glob,NOEUD));
      }
    }

    /* ------------------------------Fin du traitement des options----------------------------- */


    /* ---------------------------------Lecture des noeuds------------------------------------- */

    m_mai->donnees_lues(m_filecpp.full_name,0,ReaderCallback::NOEUD,nom_groupe_glob,0.,cancel);   
 
    for(;;) {
      const int ret=check_bloc_end(fp);

//      if(ret==1)
      if ((1 == ret) || (3 == ret))	// CP V 6.27.0
		break;

      if(ret==2) {
	getline(fp,line);
	n_ligne++;
	continue;
      }

      getline(fp,line);
      n_ligne++;

      if(fp.eof()) break;

      if(!empty_line(line)) {
	istringstream fsp(line);
 
	istream_iterator<int> it(fsp);
	istream_iterator<char> itch(fsp);
	istream_iterator<double> itdb(fsp),eos;

	while(itdb!=eos) {
	  coords.push_back(*itdb);
	  itch++;itdb++;
	}


        if(coords.size()>maillage_dim || !maillage_dim)
           maillage_dim=coords.size();
           

	if(coords.size()==1) {
	  coords.push_back(0.);
	  coords.push_back(0.);
	}

	if(coords.size()==2) 
	  coords.push_back(0.); 

	if(coords.size()==0 || coords.size()>3) 
	  throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : nombre de coordonnées incorrect.\n");

		// CP v 6.3.3 : sz est une somme et peut donc être nulle bien qu'étant
		// réellement de dimension 3 ...
		// On somme donc les valeurs absolues de coords [2]. On loupe ici le cas
		// ou tout est dans un même plan d'altitude différente de 0, mais
		// quelque part c'est du 3D.
//         sy=sy+coords[1];
//         sz=sz+coords[2];
		sy = sy + fabs (coords [1]);	// CP v 6.3.3
		sz = sz + fabs (coords [2]);	// CP v 6.3.3
         
	// Prise en compte d'un changement de repere eventuel
	if (new_system.size())
	  system_coordinates(coords[0],coords[1],coords[2]);

	// Prise en compte d'une translation/rotation eventuelle de l'instance courante
	if(trans_rot.size())
	  transform_coordinates(coords[0], coords[1], coords[2], trans_rot);
	      
	_NoeudInterne* node=_NoeudInterne::create(nbnodes_glob+*it,coords[0],coords[1],coords[2]);
               
	nbnodes_inst++;
	m_mai->ajouter(node);

	// si option NSET active -> on ajoute le noeud au nuage
	if(itopt<params.end()) {
	  m_mai->nuage(nom_groupe_glob)->ajouter(node);
	}

	it++;
	coords.clear();
      }

    }
    trans_rot.clear();

	if(!dim_lu && maillage_dim)
	{
		if(sz==0.)
		{
			maillage_dim=2;
			if(sy==0.)
				maillage_dim=1;
		}	// if(sz==0.)

		m_mai->dimension((dim_t)maillage_dim);
		dim_lu=true;
	}	// if(!dim_lu && maillage_dim)

    m_mai->donnees_lues(m_filecpp.full_name,0,ReaderCallback::NOEUD,nom_groupe_glob,100.,cancel);

}

//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Ajout d'un groupe au maillage en fonction du type d'élément
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::ajouter_groupe(const int& typ_elem,string& nom_groupe,_MaillageInterne*& m_mai)
{

  switch(typ_elem){
  case NOEUD:
    m_mai->ajouter(new _NuageInterne(nom_groupe));
    break;
	
  case BRAS:
    m_mai->ajouter(new _LigneInterne(nom_groupe)); 
    break;
	
  case POLYGONE:
    m_mai->ajouter(new _SurfaceInterne(nom_groupe)); 
    break;
	
  case POLYEDRE:
    m_mai->ajouter(new _VolumeInterne(nom_groupe)); 
    break;
	
  default:
    throw read_erreur("Erreur ligne "+to_str(n_ligne)+". Impossible d'ajouter le groupe "+nom_groupe+" au maillage : type d'élément inconnu.\n");
    break;  
  }
    
}
//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Ajout d'un élément d'un groupe existant en fonction de son type 
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::ajouter_element(int id,const int& typ_elem,string& nom_groupe,_MaillageInterne*& m_mai)
{

  switch(typ_elem) {
  case NOEUD:
    m_mai->nuage(nom_groupe)->ajouter(m_mai->noeud_id(id));
    break;
	
  case BRAS:
    m_mai->ligne(nom_groupe)->ajouter(m_mai->bras_id(id));
    break;
	
  case POLYGONE:
    m_mai->surface(nom_groupe)->ajouter(m_mai->polygone_id(id));
    break;
	
  case POLYEDRE:
    m_mai->volume(nom_groupe)->ajouter(m_mai->polyedre_id(id));
    break;
	
  default:
    throw read_erreur("Erreur ligne "+to_str(n_ligne)+". Impossible d'ajouter l'élément "+to_str(id)+" au maillage : type inconnu.\n");
    break;
  }
    
}

//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/                                  
/*!
  \brief Extraction de la peau d'un groupe en fonction de son type
  et d'un numéro de face local
  - fid : numéro de la face (1<=fid<=6)                                    
  - type_elem : type d'élément dans le groupe                              
  - nom_groupe : nom du groupe                                             
  - nom_peau : nom de la peau générée                                      
  -> si type_elem=POLYGONE : peau=LIGNE                     
  -> si type_elem=POLYEDRE : peau=SURFACE                   
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::extraire_peau( long fid,
                                   const int& type_elem,
                                   string& nom_groupe,
                                   string& nom_peau,
                                   _MaillageInterne*& m_mai)
{
 

  int face_id;
  int type;
   
  switch(type_elem) {
  case POLYGONE:

    for(int ielem=0;ielem<m_mai->surface(nom_groupe)->nb_elm();ielem++) {
      type=m_mai->surface(nom_groupe)->polygone(ielem)->nb_noeuds()+1;
      face_id=lima_faces[type][fid];

      _BrasInterne* nwbras=m_mai->surface(nom_groupe)->polygone(ielem)->extraire_arete(face_id);
      m_mai->ajouter(nwbras);
      m_mai->ligne(nom_peau)->ajouter(nwbras);
    }    
    break;

  case POLYEDRE:

    for(int ielem=0;ielem<m_mai->volume(nom_groupe)->nb_elm();ielem++) {
      type=m_mai->volume(nom_groupe)->polyedre(ielem)->type(); 
      face_id=lima_faces[type][fid];

      _PolygoneInterne* nwpolygone=m_mai->volume(nom_groupe)->polyedre(ielem)->extraire_face(face_id);
      m_mai->ajouter(nwpolygone);
      m_mai->surface(nom_peau)->ajouter(nwpolygone);
    }

    break;

  default:
    throw read_erreur("Erreur ligne "+to_str(n_ligne)+". Impossible d'ajouter la surface "+nom_peau+" au maillage : type d'élément inconnu.\n");
    break;
  }

}


void _ReaderAbaqus::extraire_face (	// CP v 6.3.3
		long maille_id, int fid, const string& surface, int dim,
		_MaillageInterne*& mesh)
{
	switch (dim)
	{
		case 2	:
		{
			int	type	= m_mai->polygone_id (maille_id)->nb_noeuds ( ) + 1;
			int	face_id	= lima_faces [type][fid];
			_BrasInterne*	b	=
					m_mai->polygone_id (maille_id)->extraire_arete (face_id);
			m_mai->ajouter (b);
			m_mai->ligne (surface)->ajouter (b);
		}
		break;
		case 3	:
		{
			int	type	= m_mai->polyedre_id (maille_id)->type ( );
			int	face_id	= lima_faces [type][fid];
			_PolygoneInterne*	pg	=
					m_mai->polyedre_id (maille_id)->extraire_face (face_id);
			m_mai->ajouter (pg);
			m_mai->surface (surface)->ajouter (pg);
		}
		break;
		default	:
			throw read_erreur ("Extraction de contour impossible pour une maille de dimension " + to_str (dim) + ".");
	}	// switch (dim)
}	// _ReaderAbaqus::extraire_face


//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Lecture d'information sur plusieurs lignes
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::read_lines(ifstream& fp, string& line)
{
  bool end_line=false;
  string tmp_line;
  char last_char;

  line="";

  for(;;){
    getline(fp,tmp_line);
    n_ligne++;
    last_char=tmp_line[tmp_line.length()-1];
	
    line=line+tmp_line; 
	
    if(last_char!=',') 
      break;       
	
  } 
}
//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief  Lecture des éléments du maillage (bloc ELEMENT) :                            
  - traitement des options passées au mot clef ELEMENT (vecteur params).     
  - création d'un groupe si l'option ELSET est active.                      
  - création d'un groupe lié à l'instance appelante si elle existe
  - lecture des numéros des noeuds.                                   
  - ajout des éléments au maillage et au groupe si l'option ELSET est active.  
	Retourne la dimension des éléments lus.
*/
/*--------------------------------------------------------------------------*/
// CP v 6.3.3 : retour de la dimension du type d'éléments lus.
int _ReaderAbaqus::lire_elements(ifstream& fp,vector<string>& params,string nom_instance_appelante)
{

  string nom_groupe;
  string nom_groupe_glob="";
  string line,abaqus_elem;
  string nom_instance;
  string nom_instance_glob="";

  int e_id;
  bool cancel=false;
  vector<int> vec;
  int type_elem	= -1;   //type d'element
  int compt=0;
  bool found=false;

  // options entrainant le non traitement de la commande
  vector<string> ignore_options;
  ignore_options.push_back("PRINT");
  ignore_options.push_back("OUTPUT");
  ignore_options.push_back("FILE");

  // options interdites
  vector<string> bad_options;
  bad_options.push_back("INPUT"); 


    /* Traitement des options */

    for(int i=0;i<ignore_options.size();i++){
      vector<string>::iterator it=find(params.begin(),params.end(),ignore_options[i]);
      if(it<params.end())
//	return;
	return -1;	// CP v 6.3.3
    }

    for(int i=0;i<bad_options.size();i++){
      vector<string>::iterator it=find(params.begin(),params.end(),bad_options[i]);
//      if(it<params.end()) 
	if (params.end ( ) != it)	// // CP, v 6.27.1, coverity
		throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : " + *it+", option de ELEMENT non traitée.\n");
    }

    // option TYPE
    vector<string>::iterator ittype=find(params.begin(),params.end(),"TYPE");

    if(ittype<params.end()) {
      abaqus_elem=*(ittype+1);

      int elem_comp	= 0;
      for(map<string,int>::iterator it=types_elements.begin();it!=types_elements.end();it++){             
	string elem_ref=it->first;
	elem_comp=abaqus_elem.compare(0,elem_ref.length(),elem_ref);
	if(elem_comp==0) {
	  type_elem=it->second;
	  break;
	}            
      }

      if(elem_comp!=0) 
	throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : les éléments Abaqus de type "+abaqus_elem+" ne sont pas traités.\n");            
    } else {
      throw read_erreur("Erreur  ligne "+to_str(n_ligne)+" : impossible de lire le type d'élément.\n");
    }

    nom_instance = nom_instance_appelante;

    // Création d'un groupe correspondant à l'instance 
    if(!nom_instance.empty()){
      nom_instance_glob=nom_instance+"_"+nom_groupe+"_INSTANCE";

      if(!contient_groupe(m_mai,nom_instance_glob,type_elem)){
        ajouter_groupe(type_elem,nom_instance_glob,m_mai);
        groupe_abaqus.insert(pair<string,string>(nom_instance,nom_instance_glob));
        types_groupes.insert(pair<string,int>(nom_instance_glob,type_elem));
      }
    }


    // option ELSET
    vector<string>::iterator itopt=find(params.begin(),params.end(),"ELSET");
     
    if(itopt<params.end()){
      nom_groupe=*(itopt+1);
      nom_groupe_glob=nom_instance+"_"+nom_groupe+"_ELSET";
        
      if(!contient_groupe(m_mai,nom_groupe_glob,type_elem)){
	ajouter_groupe(type_elem,nom_groupe_glob,m_mai); 
	groupe_abaqus.insert(pair<string,string>(nom_groupe,nom_groupe_glob));
	types_groupes.insert(pair<string,int>(nom_groupe_glob,type_elem));
      }
    }


    ////////////////* Lecture des elements *///////////////////

    m_mai->donnees_lues(m_filecpp.full_name,0,ReaderCallback::GROUPE,nom_groupe_glob,0.,cancel);
     
    for(;;){

      const int ret=check_bloc_end(fp);

//      if(ret==1)
      if ((1 == ret) || (3 == ret))	// CP V 6.27.0
		break;

      if(ret==2){
	getline(fp,line);
	n_ligne++;
	continue;
      }

      read_lines(fp,line);

      if(fp.eof()) break;

      if(!empty_line(line)){
	char end_line=line[line.length()-1];

	istringstream fsp(line);

	istream_iterator<int> it(fsp),eos;
	istream_iterator<char> itch(fsp);
              
	while(it!=eos){
	  vec.push_back(*it);
	  it++;itch++;
	}
  
	/* Ajout de l'element e_id (numero global) au maillage en fonction de sa categorie */
 
	e_id=vec.at(0)+nbelems_glob;
	type_element_lima.insert(pair<int,int>(e_id,type_elem));
              
	for(int i=1;i<vec.size();i++) 
	  vec.at(i)=vec.at(i)+nbnodes_glob;
              
	switch(type_elem) {
	case BRAS:
           
	  if(vec.size()!=3) 
	    throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : les éléments Abaqus de type "+abaqus_elem+" ne sont pas traités.\n");    

	  ajouter_bras(m_mai,vec.begin()+1,vec.end(),size_type(e_id)); 
	  nbelems_inst++;
                   
	  if(itopt<params.end()) 
	    m_mai->ligne(nom_groupe_glob)->ajouter(m_mai->bras_id(e_id));

          if(!nom_instance.empty())
            m_mai->ligne(nom_instance_glob)->ajouter(m_mai->bras_id(e_id));

	  break;

	case POLYGONE:

	  if(vec.size()!=4 && vec.size()!=5)
	    throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : les éléments Abaqus de type "+abaqus_elem+" ne sont pas traités.\n");

	  ajouter_polygone(m_mai,vec.begin()+1,vec.end(),size_type(e_id)); 
	  nbelems_inst++;

	  if(itopt<params.end())
	    m_mai->surface(nom_groupe_glob)->ajouter(m_mai->polygone_id(e_id));

          if(!nom_instance.empty())
            m_mai->surface(nom_instance_glob)->ajouter(m_mai->polygone_id(e_id));

	  break;
                              
	case POLYEDRE:

	  if(vec.size()!=5 && vec.size()!=7 && vec.size()!=9)
	    throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : les éléments Abaqus de type "+abaqus_elem+" ne sont pas traités.\n");

	  ajouter_polyedre(m_mai,vec.begin()+1,vec.end(),size_type(e_id));
	  nbelems_inst++;

	  if(itopt<params.end())
	    m_mai->volume(nom_groupe_glob)->ajouter(m_mai->polyedre_id(e_id));

          if(!nom_instance.empty())
             m_mai->volume(nom_instance_glob)->ajouter(m_mai->polyedre_id(e_id));

	  break;
                      
	default:
	  throw read_erreur("Erreur ligne "+to_str(n_ligne)+". Impossible d'ajouter l'élément "+to_str(e_id)+" au maillage : type inconnu.\n");             
	  break;

	}

	vec.clear();
      }

    }
    m_mai->donnees_lues(m_filecpp.full_name,0,ReaderCallback::GROUPE,nom_groupe_glob,100.,cancel);
    if(cancel) throw _Reader::LectureAnnulee();

	switch(type_elem)	// CP v 6.3.3
	{
		case BRAS		:	return 1;
		case POLYGONE	:	return 2;
		case POLYEDRE	:	return 3;
	}	// switch(type_elem)

	return 0;
}
//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Création d'un groupe par fusion de groupes existants
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::merge_groupe(const string& nom_groupe_in,const int& type,const string& nom_groupe)
{

  switch(type){
  case NOEUD:
           
    for(int inode=0;inode<m_mai->nuage(nom_groupe_in)->nb_elm();inode++)
      m_mai->nuage(nom_groupe)->ajouter(m_mai->nuage(nom_groupe_in)->noeud(inode));

    break;

  case BRAS:

    for(int ielem=0;ielem<m_mai->ligne(nom_groupe_in)->nb_elm();ielem++)
      m_mai->ligne(nom_groupe)->ajouter(m_mai->ligne(nom_groupe_in)->bras(ielem));
    break;

  case POLYGONE:

    for(int ielem=0;ielem<m_mai->surface(nom_groupe_in)->nb_elm();ielem++)
      m_mai->surface(nom_groupe)->ajouter(m_mai->surface(nom_groupe_in)->polygone(ielem));
 
    break;

  case POLYEDRE:

    for(int ielem=0;ielem<m_mai->volume(nom_groupe_in)->nb_elm();ielem++)
      m_mai->volume(nom_groupe)->ajouter(m_mai->volume(nom_groupe_in)->polyedre(ielem));

    break;

  default:
    throw read_erreur("Erreur ligne "+to_str(n_ligne)+". Impossible de ajouter le groupe "+nom_groupe_in+" au groupe"+nom_groupe+" : type d'élément inconnu.\n");
    break;
  }


}
//////////////////////////////////////////////////////////////////////////////
void _ReaderAbaqus::convert_string_to_long(string str_in,const int base, long& val)
{
  char* endptr;
  errno=0;
  val  = strtol(str_in.c_str(),&endptr,base);

  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
      || (errno != 0 && val == 0)
      || (endptr ==str_in.c_str()) ) {
	
    throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : les données lues ne sont pas de type numérique .\n");   
  }
    
}
//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Lecture des composants d'un groupe, bloc NSET ou ELSET
  - traitement des options passées  (vecteur params).     
  - lecture des élements 
  -> lecture standard
  -> GENERATE
  -> fusion 
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::lire_groupe(ifstream& fp,vector<string>& params,string nom_instance_appelante)
{

  string line;
  string nom_groupe="";       // Nom du groupe Abaqus lu (ex : ELSET=nom_groupe)
  string nom_groupe_glob;     // Nom global du groupe (avec le nom de l'instance en prefixe)
  string nom_instance="";
  string keyword=params.at(0);
  int type,type_groupe_abaqus;
  bool cancel=false;   
  
  int delta_node=0;
  int delta_element=0;
  vector<string> group_data;
  int delta;
  char* endptr;


    /* Taitement des options */

    // option ELSET ou NSET
    vector<string>::iterator itopt=find(params.begin()+1,params.end(),keyword);

//    if(itopt<params.end()){	//CP, v 6.27.1, coverity
    if (itopt != params.end  ( )){
		const vector<string>::iterator	itn	= itopt + 1;//CP, v 6.27.1, coverity
		if (params.end ( )	== itn)
			throw erreur ("Erreur interne du lecteur.");	// CP, v 6.27.1, coverity
//      nom_groupe=*(itopt+1);
		nom_groupe	= *itn;	// CP, v 6.27.1, coverity
      type_groupe_abaqus=abaqus_keywords[*itopt];
    }
    else 
      throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : impossible de lire le nom du groupe.\n");


    // options INSTANCE
    vector<string>::iterator optinst=find(params.begin()+1,params.end(),"INSTANCE");


    if (nom_instance_appelante != "")
      nom_instance = nom_instance_appelante;
    else
      if(optinst<params.end())
	nom_instance=*(optinst+1);

    nom_groupe_glob=nom_instance+"_"+nom_groupe+"_"+keyword;
    groupe_abaqus.insert(pair<string,string>(nom_groupe,nom_groupe_glob));


    // option GENERATE
    bool optgen=(find(params.begin(),params.end(),"GENERATE")<params.end()) || 
                (find(params.begin(),params.end(),"GEN")<params.end()) ;

    /////////////* Lecture des elements ou noeuds du groupe  *//////////////////
    m_mai->donnees_lues(m_filecpp.full_name,0,ReaderCallback::GROUPE,nom_groupe_glob,0.,cancel);

    if (nom_instance != ""){
         
       if(instance_abaqus.find(nom_instance)==instance_abaqus.end())
          throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : l'instance "+nom_instance+" n'est pas défnie.\n");
      
       else { 
           pair<mmap_it_int,mmap_it_int>  pmap=instance_abaqus.equal_range(nom_instance);
           delta_node=pmap.first->second;
           delta_element=(++pmap.first)->second;
       }
    }

    if(*itopt=="ELSET")
      delta=delta_element;
    else
      delta=delta_node;

    for(;;){
      const int ret=check_bloc_end(fp);

//      if(ret==1)
      if ((1 == ret) || (3 == ret))	// CP V 6.27.0
		break;

      if(ret==2){
	getline(fp,line);
	n_ligne++;
	continue;
      }

      getline(fp,line);
      n_ligne++;

      if(fp.eof()) break;
         
      if(!empty_line(line)){
	split_string(line," ,\r\n",group_data);

	// option generate
	if(optgen) {
	  long debut,fin,pas;                         
	  convert_string_to_long(group_data[0],10,debut);
	  convert_string_to_long(group_data[1],10,fin);
             
	  if(group_data.size()==2)
	    pas=1;
	  else 
	    convert_string_to_long(group_data[2],10,pas);

	  // type du premier element de la ligne
	  type=getElemType(debut+delta,type_groupe_abaqus);

	  if(!contient_groupe(m_mai,nom_groupe_glob,type)){
	    ajouter_groupe(type,nom_groupe_glob,m_mai);
	    types_groupes.insert(pair<string,int>(nom_groupe_glob,type));  // stockage du type de groupe
	  }
                    

	  for(int i = debut; i <= fin; i+=pas)
	    ajouter_element(i+delta,type,nom_groupe_glob,m_mai);

                     
                  
	} else { // lecture standard
			    
				
	  for(int i=0;i<group_data.size();i++){
	    if(num_string(group_data[i])){
	      long ind;
	      convert_string_to_long(group_data[i],10,ind);
	      type=getElemType(ind+delta,type_groupe_abaqus);
                      
	      if(!contient_groupe(m_mai,nom_groupe_glob,type)){
		ajouter_groupe(type,nom_groupe_glob,m_mai);
		types_groupes.insert(pair<string,int>(nom_groupe_glob,type));  // stockage du type de groupe
	      }

	      ajouter_element(ind+delta,type,nom_groupe_glob,m_mai);

	    } else { // nom de groupe --> fusion
						
	      string nomgr=group_data[i];
	      // On teste si le nom du groupe est de la forme [assembly.]nom_instance.nom_groupe
	      vector<string> vec;
	      split_string(nomgr,".",vec);
	      const short taille = vec.size();
              string nomgr_glob;

	      if (taille>1){
		  nomgr = vec[taille-1];
		  nom_instance = vec[taille-2];		  
                  nomgr_glob=nom_instance+"_"+nomgr+"_"+keyword;
	      }
              else{
                  if(nom_instance!=""){
                     nomgr_glob=nom_instance+"_"+nomgr+"_"+keyword;
                  }
                  else{
                  pair<mmap_it_str,mmap_it_str> pmap=groupe_abaqus.equal_range(nomgr);
                  nomgr_glob=pmap.first->second;
                  }
              }
	    

              if(groupe_abaqus.find(nomgr)==groupe_abaqus.end()) {
		  
		  throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : le groupe "+nomgr+" n'est pas défni.\n"); 
		  
	      } else {

		if(nomgr==nom_groupe && taille<2){                   
		  size_type nbr=groupe_abaqus.count(nomgr);

		  if(nbr>1)
		    nom_groupe_glob=nom_groupe_glob+"_"+to_str(nbr-1);

		}
		if(!contient_groupe(m_mai,nom_groupe_glob,types_groupes[nomgr_glob])){
		  ajouter_groupe(types_groupes[nomgr_glob],nom_groupe_glob,m_mai);
		  types_groupes.insert(pair<string,int>(nom_groupe_glob,types_groupes[nomgr_glob]));  // stockage du type de groupe
		}

   
		merge_groupe(nomgr_glob,types_groupes[nomgr_glob],nom_groupe_glob);
	      }
	    }
                  
	  }

	}

	group_data.clear();
      }

    }

    m_mai->donnees_lues(m_filecpp.full_name,0,ReaderCallback::GROUPE,nom_groupe_glob,100.,cancel);


}

//////////////////////////////////////////////////////////////////////////////
void _ReaderAbaqus::split_string(string& str_in,const string& sep,vector<string>& words)
{
  string keyword;
  string keyword2="";
  string::size_type n1=0;
  string::size_type n2=1;
  while(int(n2)>=0){

    n2=str_in.find_first_of(sep,n1);
    
    if(n2-n1>0){ 
      keyword=str_in.substr(n1,n2-n1);
      transform(keyword.begin(),keyword.end(),keyword.begin(),majuscule());

      if(keyword.length()){

        if(keyword.at(0)=='"' && keyword.at(keyword.length()-1)=='"'){
	  words.push_back(keyword);           
	}
        else{

	  if(keyword.at(0)=='"'){
	    keyword.erase(keyword.begin()); 
	    keyword2=keyword2+"_"+keyword;
	  }
	  else{
         
	    if(keyword.at(keyword.length()-1)=='"'){
	      keyword.erase(keyword.end()-1);
	      keyword2=keyword2+"_"+keyword;
	      words.push_back(keyword2);
	      keyword2="";
	    }
	    else{
	      if(!keyword2.empty()){
		keyword2=keyword2+"_"+keyword;
	      }
	      else
		words.push_back(keyword); 
	    }
          }
        }
      }
    }
    n1=n2+1;
  }
}
//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Lecture des composants d'une surface, bloc SURFACE
  - traitement des options passées  (vecteur params).     
  - Création de la surface (ou ligne) par extraction
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::lire_surface(ifstream& fp,vector<string>& params,string nom_instance_appelante, int dim)
{
  string line;
  string nom_groupe;      // nom Abaqus du groupe (ELSET) associe a la surface 
  string nom_groupe_glob; // Nom Lima du groupe
  string face_name;       // identifiant abaqus de la face (S1,...,S6)
  string type_element;    // type d'element constituant la surface (valeur par defaut : ELEMENT)
  string nom_surface="";  // nom de la surface 
  string nom_instance;

  int surf_type;          // type associe a la surface
  long face_id ;           // numero de la face Abaqus a extraire
  int nbgroups=0;         // nombre de groupes qui composent la surface
  int base=10;
  char* endptr;
  vector<string> vec;
  bool cancel=false;
 
  // options entrainant le non traitement de la commande
  vector<string> ignore_options;
  ignore_options.push_back("INTERACTION");
  ignore_options.push_back("PROPERTY");
  ignore_options.push_back("BEHAVIOR");

  // options interdites
  vector<string> bad_options;
  bad_options.push_back("TRIM"); 
  bad_options.push_back("CROP"); 
  bad_options.push_back("COMBINE"); 
  bad_options.push_back("DEFINITION"); 
  bad_options.push_back("FILLET"); 
  bad_options.push_back("REGION"); 
  bad_options.push_back("RATIO"); 
  bad_options.push_back("OFFSET"); 
  bad_options.push_back("THICK"); 
 


    for(int i=0;i<ignore_options.size();i++){
      vector<string>::iterator it=find(params.begin(),params.end(),ignore_options[i]);
      if(it<params.end())
	return;
    }

    // option TYPE
    vector<string>::iterator ittype=find(params.begin(),params.end(),"TYPE");

    if(ittype<params.end()){
      type_element=*(ittype+1);

	if(type_element.compare("ELEMENT"))
	{	// CP v 6.13.1 : ce cas n'est plus bloquant, on ne le lit pas.
	    vector<string>::iterator	itname =
								find (params.begin ( ),params.end ( ),"NAME");
		if (params.end ( ) != itname)
			itname++;
		if (params.end ( ) != itname)
			nom_surface = *itname;
//		std::cerr << "Surface \"" << *itname << "\" non lue en ligne "
		// CP, v 6.27.1, coverity :
		std::cerr << "Surface \"" << nom_surface << "\" non lue en ligne "
		          << n_ligne << " : surfaces de types autres qu'\"ELEMENT\" "
		          << "non supportées." << std::endl;

		int	ret	= check_bloc_end (fp);
		while (1 != ret)
		{
			getline (fp, line);
			n_ligne++;
			ret = check_bloc_end (fp);
			if (true == fp.eof ( ))
				return;
			if (true == fp.bad ( ))
				throw read_erreur ("Erreur ligne " + to_str (n_ligne) +
				                   " : fichier invalide.");
		}	// while (1 != ret)
//	throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : les surfaces doivent etre de type ELEMENT.\n");	// CP v 6.13.1
	}	// if(type_element.compare("ELEMENT"))
    }


    // option NAME
    vector<string>::iterator itname=find(params.begin(),params.end(),"NAME");
    nom_instance=nom_instance_appelante;

    if(itname<params.end()) {
      nom_surface=*(itname+1);
      nom_surface=nom_instance+"_"+nom_surface;
    } 
    else 
      throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : impossible de lire le nom de la surface.\n");
       
    for(int i=0;i<bad_options.size();i++){
      vector<string>::iterator it=find(params.begin(),params.end(),bad_options[i]);
//      if(it<params.end()) 
	if (params.end ( ) != it)	// // CP, v 6.27.1, coverity
		throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : " + *it+", option de SURFACE non traitée.\n");
    }


    ///////////////* Lectue des composantes des la surface *//////////////////
    m_mai->donnees_lues(m_filecpp.full_name,0,ReaderCallback::GROUPE,nom_surface,0.,cancel);


    for(;;){
      const int ret=check_bloc_end(fp);

//      if(ret==1)
      if ((1 == ret) || (3 == ret))	// CP V 6.27.0
		break;

      if(ret==2){
	getline(fp,line);
	n_ligne++;
	continue;
      }

      getline(fp,line);
      n_ligne++;
      if(fp.eof()) break;


      if(!empty_line(line)){
	split_string(line," ,\r\n",vec);            

        if(vec.size()<2){
          MutableString   message;
          message << "Erreur ligne "+to_str(n_ligne)+".\n"
                  << "Le nom du groupe ou/et le numéro de la face n'est pas défini.\n";

          throw read_erreur(message);
        }

        nom_groupe=vec.at(0);                // nom du groupe (ELSET)

	// CP : Faut-il extraire la surface (peau/contour) de toutes les mailles
	// d'un groupe, ou juste une face d'une maille des polyèdres du maillage ?
	bool	extractAll	= true;	// CP v 6.3.3
	if(groupe_abaqus.find(nom_groupe)==groupe_abaqus.end())
	{	// CP v 6.3.3 : on extraiera juste la face d'une maille.
//	  throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : le groupe "+nom_groupe+" n'existe pas.\n");	// CP v 6.3.3 -> commentaire
		extractAll	= false;	// CP v 6.3.3
	}	// if(groupe_abaqus.find(nom_groupe)==groupe_abaqus.end())

	pair<multimap<string,string>::iterator,multimap<string,string>::iterator>
								pmap=groupe_abaqus.equal_range(nom_groupe);
	if (groupe_abaqus.end ( ) == pmap.first)
		throw read_erreur ("Erreur en ligne " + to_str(n_ligne) +
			". Extraction de surface : groupe " + nom_groupe + " non recensé.");
	nom_groupe_glob=pmap.first->second;

	if(groupe_abaqus.count(nom_groupe)>1) {

	  // 				cout<<"Attention ! "<<endl;
	  // 				cout<<"Generation de la surface "+nom_surface+" (ligne "+to_str(n_ligne)+"). ";
	  // 				cout<<"Le nom "+nom_groupe+" est ambigu car il designe plusieurs groupes : "<<endl;

	  for(multimap<string,string>::iterator i=pmap.first;i!=pmap.second;i++){
	    // 					cout<<"-> "<<i->second<<endl;
                                                                                    
	    if(types_groupes[i->second]==POLYGONE || types_groupes[i->second]==POLYEDRE)
	      nom_groupe_glob=i->second;               
	  }
               
	  // 				cout<<"Prise en compte du groupe : "+nom_groupe_glob<<endl;
	}
                
              
	if(nbgroups==0){
		  surf_type=types_groupes[nom_groupe_glob];  // type d'element du groupe (POLYEDRE,POLYGONE)
	  ajouter_groupe(surf_type-1,nom_surface,m_mai);
	}
	nbgroups++;
        
	face_name=vec.at(1).substr(1);

	convert_string_to_long(face_name,10,face_id); 
	face_id--;              

	if(face_id<0) {
	  MutableString   message;
	  message << "Erreur ligne "+to_str(n_ligne)+".\n"
		  << "L'identifiant de la face est erroné ("+vec.at(1)+").\n"
		  << "Les identifiants reconnus sont S1, S2, S3, S4, S5, S6.\n";
 
	  throw read_erreur(message);
	}

	if (true == extractAll)	// CP v 6.3.3
	{
		// on extrait la face 'face_id' du groupe en fonction du type d'element 
		extraire_peau(face_id,surf_type,nom_groupe_glob,nom_surface,m_mai);
	}	// if (true == extractAll)
	else
	{
		long	maille_id	= -1;
		convert_string_to_long (vec.at (0), 10, maille_id); 
		extraire_face (maille_id, face_id, nom_surface, dim, m_mai);
	}	// if (true == extractAll)

	vec.clear();
      }
 
    }
    m_mai->donnees_lues(m_filecpp.full_name,0,ReaderCallback::GROUPE,nom_surface,100.,cancel);


}
//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Calcul des coordonnées d'un point dans l'ancien repère,
  Nouveau système de coordonnées défini par les points a,[b,[c]] du bloc SYSTEM
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::system_coordinates(double& x,double& y, double& z)
{
  const int taille = new_system.size();   
  const double xa  = new_system[0], ya = new_system[1], za = new_system[2];

  // Un seul point défini : translation pure
  if (taille == 3) {
    x = x + xa;
    y = y + ya;
    z = z + za;
    return;
  }

  // Coordonnées du point dans le nouveau repère
  double Pprim [3] = {x, y, z}; 
  double P     [3] = {0.,0.,0.};

  // Deux ou trois points définis, on utilise la matrice de passage
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      P[i] += matrice[i][j] * Pprim[j];

  //Coordonnes du point dans l'ancien repère
  x = P[0] + xa;
  y = P[1] + ya;
  z = P[2] + za;
   
}

//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Calcul des coordonnées d'un point lorsque l'instance courante
  a des paramètres de translation / rotation
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::transform_coordinates(double& x,double& y, double& z,
					  vector<double>& vec)
{
  double xp, yp,zp;
  const double x0 = vec[0],y0 = vec[1], z0 = vec[2];
  
  // Translation pure
  if(vec.size()<=3) {
    x = x + x0;
    y = y + y0;
    z = z + z0;
  } else {// Translation + Rotation
	
      
    const double a = vec[3], b = vec[4], c = vec[5];
    const double d = vec[6], e = vec[7], f = vec[8];
    const double PI = 2*asin(1.);
    const double theta = vec[9] * PI / 180.;
      
      
    // Translation
    const double xt = x + x0;
    const double yt = y + y0;
    const double zt = z + z0;


    const double u = d - a, v = e - b , w = f - c;
    const double u2 = u*u, v2 = v*v, w2 = w*w;
    const double norm  = u2 + v2 + w2;
    const double sqrtnorm  = sqrt(norm);

    const double au = a*u, av = a*v, aw = a*w;
    const double bu = b*u, bv = b*v, bw = b*w;
    const double cu = c*u, cv = c*v, cw = c*w;

    const double ux = u*xt, uy = u*yt, uz = u*zt;
    const double vx = v*xt, vy = v*yt, vz = v*zt;
    const double wx = w*xt, wy = w*yt, wz = w*zt;

    const double costheta = cos(theta), sintheta = sin(theta);

    // Rotation
    xp = a*(v2+w2)+u*(-bv-cw+ux+vy+wz)+( (xt-a)*(v2+w2)+ u*(bv+cw-vy-wz) )*costheta + sqrtnorm*(bw-cv-wy+vz)*sintheta;
    xp /= norm;

    yp = b*(u2+w2)+v*(-au-cw+ux+vy+wz)+( (yt-b)*(u2+w2)+ v*(au+cw-ux-wz) )*costheta + sqrtnorm*(-aw+cu+wx-uz)*sintheta;
    yp /= norm;

    zp = c*(u2+v2)+w*(-au-bv+ux+vy+wz)+( (zt-c)*(u2+v2)+ w*(au+bv-ux-vy) )*costheta + sqrtnorm*(av-bu-vx+uy)*sintheta;
    zp /= norm;

    x=xp;
    y=yp;
    z=zp;
  }
} 
//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Lecture de coordonnés (pour SYSTEM ou INSTANCE)
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::lire_point(ifstream& fp,vector<double>& coords)
{
	if ((true == fp.bad ( )) || (true == fp.eof ( )))
		throw read_erreur ("Erreur ligne " + to_str (n_ligne) +	// CP v 6.13.1
		                   " : fichier invalide ou fin de fichier.");
  string line;

  getline(fp,line);
  istringstream fsp(line);

	if ((true == fsp.bad ( )) || (true == fsp.eof ( )))
		throw read_erreur ("Erreur ligne " + to_str (n_ligne) +	// CP v 6.13.1
	                       " : ligne invalide (\"" + line + "\").");

  istream_iterator<double> it(fsp);
  istream_iterator<char> itch(fsp);
  istream_iterator<double> eos;

	if (true == fsp.bad ( ))
		throw read_erreur ("Erreur ligne " + to_str (n_ligne) +	// CP v 6.13.1
	                       " : ligne invalide (\"" + line + "\").");
  while(it!=eos){
    coords.push_back(*it);
    it++;
    itch++;
  }
    
  n_ligne++;
}
//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Lecture du nouveau systeme de coordonnées a, [b,[c]]
  Calcul de la matrice de passage dans l'ancien repère
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::lire_system(ifstream& fp)
{
      
  new_system.clear();
  string line;    
  int ret;
  for(;;){
    ret=check_bloc_end(fp);
	
//    if(ret==1)
      if ((1 == ret) || (3 == ret))	// CP V 6.27.0
      	break;
	
    if(ret==2){
      getline(fp,line);
      n_ligne++;
      continue;
    }
	
    lire_point(fp,new_system);
  }
       

  const int taille = new_system.size();
  if(taille != 0 && taille != 3 && taille != 6 && taille != 9)
    throw read_erreur("Erreur ligne "+to_str(n_ligne)+". Les paramètres de SYSTEM sont incorrects.\n");
    
  if(taille) {

    const double xa = new_system[0], ya = new_system[1], za = new_system[2];

    // Si un seul point est défini, translation pure, on ne calcule pas la matrice de passage
       
    // Deux point sont définis : le vecteur ac est positionné orthogonal à ab sur le plan (Oxy)
    // l'axe Z est inchangé
    if (taille == 6) {

      const double xb = new_system[3], yb = new_system[4], zb = new_system[5];
      const double xab = xb-xa, yab = yb-ya, zab = zb-za;
      const double normb = sqrt(xab*xab + yab*yab + zab*zab);
	   
      matrice[0][0] = xab/normb; matrice[0][1] = -yab/normb; matrice[0][2] = 0.;
      matrice[1][0] = yab/normb; matrice[1][1] =  xab/normb; matrice[1][2] = 0.;
      matrice[2][0] = 0.;        matrice[2][1] =  0.;        matrice[2][2] = 1.;
    }

    // Trois points sont définis
    if (taille == 9) {
	   
      const double xb = new_system[3], yb = new_system[4], zb = new_system[5];
      const double xc = new_system[6], yc = new_system[7], zc = new_system[8];
	   
      const double xab = xb-xa, yab = yb-ya, zab = zb-za;
      const double xac = xc-xa, yac = yc-ya, zac = zc-za;
      const double xad = yab*zac - zab*yac, yad = zab*xac - xab*zac, zad = xab*yac - yab*xac;
	   
      const double normb = sqrt(xab*xab + yab*yab + zab*zab);
      const double normc = sqrt(xac*xac + yac*yac + zac*zac);
      const double normd = sqrt(xad*xad + yad*yad + zad*zad);
	   
      matrice[0][0] = xab/normb; matrice[0][1] = xac/normc; matrice[0][2] = xad/normd;
      matrice[1][0] = yab/normb; matrice[1][1] = yac/normc; matrice[1][2] = yad/normd;
      matrice[2][0] = zab/normb; matrice[2][1] = zac/normc; matrice[2][2] = zad/normd;
    }

  } 
}
//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Vérifie l'existence d'un groupe
*/
/*--------------------------------------------------------------------------*/
bool _ReaderAbaqus::contient_groupe(_MaillageInterne* mmai,string nom_groupe,int type)
{
  bool exist=false; 

  switch(type){
  case NOEUD:

    for(int i=0;i<mmai->nb_nuages();i++){
      if(mmai->nuage(i)->nom()==nom_groupe)
	exist=true;
    }
    break;

  case BRAS:

    for(int i=0;i<mmai->nb_lignes();i++){
      if(mmai->ligne(i)->nom()==nom_groupe)
	exist=true;
    }
    break;

  case POLYGONE:

    for(int i=0;i<mmai->nb_surfaces();i++){
      if(mmai->surface(i)->nom()==nom_groupe)
	exist=true;
    }
    break;

  case POLYEDRE:

    for(int i=0;i<mmai->nb_volumes();i++){
      if(mmai->volume(i)->nom()==nom_groupe)
	exist=true;
    }
    break;
          
  default:
    throw read_erreur("Erreur ligne "+to_str(n_ligne)+". Le type d'élément du groupe "+nom_groupe+" est incorrect.\n");    
    break;  
  }	
  
  return exist;
}
//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Lecture des mots-clefs d'un bloc PART ou INSTANCE
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::lire_bloc_mots_clefs(ifstream& fp, string nom_instance)
{

  vector<string> params;
  bool lu;
  int key;
  const string end_word="END";

  for(;;){
    lu=lire_ligne(fp,params);

    if(lu){
      if(!params.at(0).compare(0,end_word.length(),end_word)){
	params.at(0)=end_word;
      }

      key=abaqus_keywords[params.at(0)];
            
      switch(key){
      case ABAQUS_END:
	if(pos_instance != pos_init)  {// on est en train de lire une PART
	  // On retourne se positionner sur l'instance 
	  fp.seekg(pos_instance);
	  return;
	}			
	return;
	break;
      case ABAQUS_NODE:
	lire_noeuds(fp,params,nom_instance);
	break;

      case ABAQUS_ELEMENT:
	lire_elements(fp,params,nom_instance);
	break;
          
      case ABAQUS_NSET:
	lire_groupe(fp,params, nom_instance);
	break;
    
      case ABAQUS_ELSET:
	lire_groupe(fp,params,nom_instance);
	break;
        
      case ABAQUS_SURFACE:
	lire_surface(fp,params,nom_instance);
	break;

      case ABAQUS_SYSTEM:
	lire_system(fp);
	break;

      case ABAQUS_NGEN:
	throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef NGEN non traité.\n"); 
	break;
 
      case ABAQUS_NCOPY:
	throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef NCOPY non traité.\n"); 
	break;

      case ABAQUS_ELGEN:
	throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef ELGEN non traité.\n"); 
	break;
 
      case ABAQUS_ELCOPY:
	throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef ELCOPY non traité.\n"); 
	break;

      case ABAQUS_NFILL:
	throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef NFILL non traité.\n");
	break;

      case ABAQUS_ORIENTATION:
	throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef ORIENTATION non traité.\n");
	break;
		  
      case ABAQUS_INCLUDE:
	throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef INCLUDE non traité.\n");
	break;

      case ABAQUS_PARAMETERS:
	throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef PARAMETERS non traité.\n");
	break;

      case ABAQUS_PARAMETER:
	throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef PARAMETER non traité.\n");
	break;

      default:
	break;                

      }
      params.clear();     
    }
  }

}
//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Lecture d'une instance
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::lire_instance(ifstream& fp,vector<string>& params)
{

  string nom_instance; // nom de l'instance
  string nom_part;     // nom de du domaine parent
  string line;
  //  vector<string> internal_params; // mots_cles Abaqus de l'instance


    nbnodes_inst=0;
    nbelems_inst=0;

    /* Traitement des options*/

    // option NAME
    vector<string>::iterator optname=find(params.begin(),params.end(),"NAME");

    if(optname<params.end()) 
      nom_instance=*(optname+1);
    else 
      throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : impossible de lire le nom de l'instance.\n");

    instance_abaqus.insert(make_pair(nom_instance,nbnodes_glob));
    instance_abaqus.insert(make_pair(nom_instance,nbelems_glob));
      
    // option PART
    vector<string>::iterator optpart=find(params.begin(),params.end(),"PART");

    if(optpart<params.end()) 
      nom_part=*(optpart+1);
    else 
      throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : impossible de lire le nom du domaine (PART).\n");


    /* Lecture des donnees de l'instance */
    for(;;) {
      const int ret=check_bloc_end(fp);

//      if(ret==1)
      if ((1 == ret) || (3 == ret))	// CP V 6.27.0
		break;

      if(ret==2){
	getline(fp,line);
	n_ligne++;
	continue;
      }

      lire_point(fp,trans_rot);
    }
    
    if(trans_rot.size()>3 && trans_rot.size()<10){
      throw read_erreur("Erreur ligne "+to_str(n_ligne)+".\n"
			"Instance : "+nom_instance+". Le nombre de paramètres de translation/rotation est incorrect.\n");
    }

    // Les infos de l'instance sont dans la PART associée = > on va lire les infos dans la PART
    if (info_part[nom_part] ) {
	
      //Position actuelle
      pos_instance = fp.tellg();
	
      //Position de la PART associée
      const ifstream::pos_type pos_part = position_part[nom_part];
	
      // On se positionne sur la PART 
      fp.seekg(pos_part);

      lire_bloc_mots_clefs(fp,nom_instance);
      pos_instance=0;        

    }else {
      // Les infos de l'instance sont dans l'instance 
      lire_bloc_mots_clefs(fp,nom_instance);
    }
 
    trans_rot.clear();
    nbnodes_glob+=nbnodes_inst;
    nbelems_glob+=nbelems_inst;

}

//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Lecture des infos d'une PART
  Utilisé simplememt pour déterminer si la PART contient des infos ou non
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::lire_part(ifstream& fp, vector<string>& params)
{
  int key;
  bool lu = false;
  bool init_tag = false;
  vector<string> part_params;
  string nom_part;
  const string end_word="END";
  const ifstream::pos_type position = fp.tellg();
 
  // option NAME
  vector<string>::iterator optname=find(params.begin(),params.end(),"NAME");
  
  if(optname<params.end()) 
    nom_part=*(optname+1);
  else 
    throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : impossible de lire le nom de la PART.\n");

 
    for(;;){
      if(fp.eof())
	break;

      lu=lire_ligne(fp,part_params);

      if(init_tag)
         n_ligne--; 
         
      if(lu){

	if(!part_params.at(0).compare(0,end_word.length(),end_word)){
	  part_params.at(0)=end_word;
	}

	key=abaqus_keywords[part_params.at(0)];

	switch(key){
	case ABAQUS_END:
	  if (! init_tag) {
	    info_part.insert(pair<string,bool>(nom_part, false));
	  }
	  return;
		    
	  break;

	case ABAQUS_NODE:
		    
	  if (! init_tag) {
	    init_tag = true;
	    info_part.insert(make_pair(nom_part, true));
	    position_part.insert(make_pair(nom_part, position));
	  }
	  break;
	default:
	  break;
	}
	part_params.clear();
      } 
    }    


}
//////////////////////////////////////////////////////////////////////////////
/*--------------------------------------------------------------------------*/
/*!
  \brief Lecture du fichier abaqus
*/
/*--------------------------------------------------------------------------*/
void _ReaderAbaqus::lire()
{

  ifstream & fcpp=m_filecpp.fp;
  vector<string> parameters;
  bool lu;
  int key;
  n_ligne=0;


	int	derniere_dim	= -1;	// CP v 6.3.3
    for(;;){
      if(fcpp.eof()) 
	break;
      
      lu=lire_ligne(fcpp,parameters);

      if(lu){      
	key=abaqus_keywords[parameters.at(0)];
           
	switch(key){
	case ABAQUS_NODE:
	  lire_noeuds(fcpp,parameters); 
	  break;

	case ABAQUS_ELEMENT:
//	  lire_elements(fcpp,parameters); 
	  derniere_dim	= lire_elements(fcpp,parameters); 	// CP v 6.3.3
	  break;

	case ABAQUS_NSET:
	  lire_groupe(fcpp,parameters);
	  break;

	case ABAQUS_ELSET:
	  lire_groupe(fcpp,parameters); 
	  break;

	case ABAQUS_SURFACE:
//	  lire_surface(fcpp,parameters);
	  lire_surface (fcpp, parameters, "", derniere_dim);
	  break;
 
	case ABAQUS_INSTANCE:
	  lire_instance(fcpp,parameters); 
	  break;

	case ABAQUS_PART:
	  lire_part(fcpp,parameters);                      
	  break;
        
	case ABAQUS_SYSTEM:
	  lire_system(fcpp);
	  break;

	case ABAQUS_NGEN:
	  throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef NGEN non traité.\n"); 
	  break;
 
	case ABAQUS_NCOPY:
	  throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef NCOPY non traité.\n"); 
	  break;

	case ABAQUS_ELGEN:
	  throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef ELGEN non traité.\n"); 
	  break;
 
	case ABAQUS_ELCOPY:
	  throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef ELCOPY non traité.\n"); 
	  break;

	case ABAQUS_NFILL:
	  throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef NFILL non traité.\n");
	  break;

	case ABAQUS_ORIENTATION:
	  throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef ORIENTATION non traité.\n");
	  break;
		  
	case ABAQUS_INCLUDE:
	  throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef INCLUDE non traité.\n");
	  break;

	case ABAQUS_PARAMETERS:
	  throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef PARAMETERS non traité.\n");
	  break;

	case ABAQUS_PARAMETER:
	  throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef PARAMETER non traité.\n");
	  break;

        case ABAQUS_IMPORT:
          throw read_erreur("Erreur ligne "+to_str(n_ligne)+" : Mot clef IMPORT non traité.\n");
          break;

	default:
	  break;
	}   
	if ((ABAQUS_ELEMENT != key) && (ABAQUS_SURFACE != key))
		derniere_dim	= -1;	// CP v 6.3.3


      } 
            
      parameters.clear();}
   
}

END_NAMESPACE_LIMA
#endif
