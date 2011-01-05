
/*
 *  poXMLparser.cpp
 *  poXMLparser
 *
 *  Created by Jared Schiffman on 2/23/07.
 *  Copyright 2007 Potion Design LLC. All rights reserved.
 *
 */

#include "poXMLparser.h"

// ============================== poXMLnode ================================= //
poXMLattribute::poXMLattribute( const char* theName, const char* theValue )
{
	init(theName, theValue);
}

// POINTERS WERE ONLY SHALLOW COPIED IN THE DEFAULT COPY CONSTRUCTOR
poXMLattribute::poXMLattribute(const poXMLattribute& rhs)
{
	init(rhs.name, rhs.value);
}

// POINTERS WERE ONLY SHALLOW COPIED IN THE DEFAULT COPY CONSTRUCTOR
poXMLattribute& poXMLattribute::operator=(const poXMLattribute& rhs)
{
	init(rhs.name, rhs.value);
	return *this;
}

poXMLattribute::~poXMLattribute()
{
	if(name)
		free(name);
	if(value)
		free(value);
}

void poXMLattribute::init(const char* n, const char* v)
{
	name = value = NULL;
	
	if ( n != NULL )
	{
		int L = strlen( n );
		name = (char*) malloc( L+1 );
		strcpy( name, n );
	}
	
	if ( v != NULL )
	{
		int L = strlen( v );
		value =  (char*) malloc( L+1 );
		strcpy( value, v );
	}
}

// ============================== poXMLnode ================================= //

	
poXMLnode::poXMLnode()
{
	init();
}

poXMLnode::poXMLnode( const char* name )
{
	init();
	setName( name );
}

poXMLnode::poXMLnode( const char* name, const char* data )
{
	init();
	setName( name );
	setData( data );
}

poXMLnode::poXMLnode( const char* name, int data )
{
	init();
	setName( name );
	setData( data );
}

poXMLnode::poXMLnode( const char* name, float data )
{
	init();
	setName( name );
	setData( data );
}

poXMLnode::~poXMLnode()
{
	if(name)
		free(name);
	if(data)
		free(data);
	
	if(attributeList)
		free(attributeList);
		
	if(childList)
	{
		for(int i=0; i<numChildren; i++)
			delete childList[i];
			
		free(childList);
	}
}
	
void	poXMLnode::init()
{
	name = NULL;
	data = NULL;
	
	attributeList = NULL;
	numAttributes = 0;
	
	numChildren = numAllocChildren = 0;
	childList = NULL;
}


void	poXMLnode::setName( const char* nodeName )
{
	if ( nodeName == NULL )
		return;
	
	int L = strlen( nodeName );
	
	name = (char*) realloc( name, sizeof(char)*L+2 );
	strcpy( name, nodeName );
}


void	poXMLnode::addAttribute( const char* attName, const char* attValue )
{
	//printf("%d %s %s\n", numAttributes, attName, attValue );
	
	poXMLattribute A( attName, attValue );
	
	numAttributes++;
	attributeList = (poXMLattribute*) realloc( attributeList, sizeof(poXMLattribute)*numAttributes );
	attributeList[ numAttributes-1 ] = A;	
}


void poXMLnode::setData(const char* nodeData )
{
	if ( nodeData == NULL )
		return;
	int L = (int)strlen( nodeData );
	data = (char*) realloc( data, sizeof(char)*L+2 );
	strcpy( data, nodeData );
}

void poXMLnode::setData( int nodeData )
{
	char str[32];
	sprintf( str, "%d", nodeData );
	setData( str );
}

void poXMLnode::setData( float nodeData )
{
	char str[32];
	sprintf( str, "%f", nodeData );
	setData( str );
}
	
	
poXMLnode*		poXMLnode::addChild( poXMLnode* N )
{
	numChildren++;
	if ( numChildren >= numAllocChildren )
	{
		numAllocChildren = numChildren*2;
		childList = (poXMLnode**) realloc( childList, sizeof(poXMLnode*)*numAllocChildren );
	}
	childList[numChildren-1] = N;
	return N;
}


int		poXMLnode::getNumNodes()
{
	return numChildren;
}


int		poXMLnode::getNumNodes( const char* nodeName )
{
	int count = 0;
	for( int i=0; i<numChildren; i++ )
	{
	   // printf("%d)  %s\n", i, 	childList[i]->name );
		if ( childList[i]->name != NULL )
			if ( strcmp( childList[i]->name, nodeName ) == 0 )
				count++;
	}
	return count;
}


poXMLnode*		poXMLnode::getNode( int N )
{
	if ( N < 0 || N >= numChildren )
		return NULL;
	return childList[N];
}


poXMLnode*		poXMLnode::getNode( const char* nodeName )
{
	for( int i=0; i<numChildren; i++ )
	{
		if ( childList[i]->name != NULL )
			if ( strcmp( childList[i]->name, nodeName ) == 0 )
				return childList[i];
	}
	
	return NULL;
}


poXMLnode*		poXMLnode::getNode( const char* nodeName, int N )
{
	int count = 0;
	for( int i=0; i<numChildren; i++ )
	{
		if ( childList[i]->name != NULL )
		{
			if ( strcmp( childList[i]->name, nodeName ) == 0 )
			{
				if ( count == N )
					return childList[i];
				count++;
			}
		}
	}
	
	return NULL;
}


poXMLnode*		poXMLnode::getNode( const char* attName, const char* attValue )
{

	for( int i=0; i<numChildren; i++ )
		if ( childList[i]->hasAttributeEqual( attName, attValue ) )
			return childList[i];
	
	return NULL;
}


int				poXMLnode::getDataAsInt()
{
	if ( data != NULL )
		return atoi( data );
	return 0;
}


float			poXMLnode::getDataAsFloat()
{
	if ( data != NULL )
		return float(atof(data));
	return 0.f;
}


const char*	poXMLnode::getDataAsString()
{
	return data;
}


int poXMLnode::getNodeAsInt( const char* nodeName )
{
	poXMLnode* N = getNode( nodeName );
	if ( N != NULL )
		return N->getDataAsInt();
	else
		return 0;
}

float		poXMLnode::getNodeAsFloat( const char* nodeName )
{
	poXMLnode* N = getNode( nodeName );
	if ( N != NULL )
		return N->getDataAsFloat();
	else
		return 0.0;
}

const char* poXMLnode::getNodeAsString( const char* nodeName )
{
	poXMLnode* N = getNode( nodeName );
	if ( N != NULL )
		return N->getDataAsString();
	else
		return NULL;
}


const char* poXMLnode::getAttribute( const char* attName )
{
	for( int i=0; i<numAttributes; i++ )
		if ( strcmp( attributeList[i].name, attName )==0 )
			return attributeList[i].value;
	return NULL;
}

bool	poXMLnode::hasAttributeEqual( const char* attName, const char* attValue )
{
	for( int i=0; i<numAttributes; i++ )
		if ( strcmp( attributeList[i].name, attName )==0  &&  strcmp( attributeList[i].value, attValue )==0 )
			return true;
	return false;
}


void	poXMLnode::unparse( int tabDepth, FILE* outFile )
{
	#ifdef WIN32
	// if no children, print in one line
	if ( numChildren == 0 )
	{
		for( int i=0; i<tabDepth*4; i++ )
			fprintf( outFile, " ");
		if ( name != NULL ) fprintf( outFile,"<%s", name );
		
		for( int i=0; i<numAttributes; i++ )
			fprintf( outFile, " %s=\"%s\"", attributeList[i].name, attributeList[i].value );
		
		fprintf( outFile, ">" );

		if ( data != NULL ) fprintf( outFile,"%s", data );

		if ( name != NULL ) fprintf( outFile,"</%s>\r\n", name );

		return;
	}
	
	// print open tag
	if ( name != NULL )
	{
		for( int i=0; i<tabDepth*4; i++ )
			fprintf( outFile," ");
		fprintf( outFile,"<%s", name );
		
		for( int i=0; i<numAttributes; i++ )
			fprintf( outFile, " %s=\"%s\"", attributeList[i].name, attributeList[i].value );
		
		fprintf( outFile, ">\r\n" );
	}
	
	// print data
	if ( data != NULL )
	{
		for( int i=0; i<tabDepth*4; i++ )
			fprintf( outFile, " ");
		fprintf( outFile, "%s\r\n", data );
	}
	
	for( int i=0; i<numChildren; i++ )
		childList[i]->unparse( tabDepth+1, outFile );
	
	// print close tag
	if ( name != NULL )
	{
		for( int i=0; i<tabDepth*4; i++ )
			fprintf( outFile, " ");
		fprintf( outFile, "</%s>\r\n", name );
	}
	#else
	// if no children, print in one line
	if ( numChildren == 0 )
	{
		for( int i=0; i<tabDepth*4; i++ )
			fprintf( outFile, " ");
		if ( name != NULL ) fprintf( outFile,"<%s", name );
		
		for( int i=0; i<numAttributes; i++ )
			fprintf( outFile, " %s=\"%s\"", attributeList[i].name, attributeList[i].value );
		
		fprintf( outFile, ">" );

		if ( data != NULL ) fprintf( outFile,"%s", data );
		if ( name != NULL ) fprintf( outFile,"</%s>\n", name );
		return;
	}
	
	// print open tag
	if ( name != NULL )
	{
		for( int i=0; i<tabDepth*4; i++ )
			fprintf( outFile," ");
		fprintf( outFile,"<%s", name );
		
		for( int i=0; i<numAttributes; i++ )
			fprintf( outFile, " %s=\"%s\"", attributeList[i].name, attributeList[i].value );
		
		fprintf( outFile, ">\n" );
	}
	
	// print data
	if ( data != NULL )
	{
		for( int i=0; i<tabDepth*4; i++ )
			fprintf( outFile, " ");
		fprintf( outFile, "%s\n", data );
	}
	
	for( int i=0; i<numChildren; i++ )
		childList[i]->unparse( tabDepth+1, outFile );
	
	// print close tag
	if ( name != NULL )
	{
		for( int i=0; i<tabDepth*4; i++ )
			fprintf( outFile, " ");
		fprintf( outFile, "</%s>\n", name );
	}

	#endif
}

// ============================== poXMLparser ================================= //


poXMLparser::poXMLparser()
{}

poXMLnode* poXMLparser::parseFile( const char* fileName )
{
	char*	fileData;
	int		fileLength;
	bool	skipFirstTag = false;
	
	FILE* theFile = fopen( fileName, "r" );

#ifdef WIN32
	int c;
	int n = 0;
	if ( theFile==NULL )
	{
		printf("ERROR: poXMLparser: could not read file %s\n", fileName );
		return NULL;
	}else
	{
		do{
			c = fgetc( theFile );
			n++;
		}while( c != EOF );			
	}
	fileLength = n;
#else
	if ( theFile==NULL )
	{
		printf("ERROR: poXMLparser: could not read file %s\n", fileName );
		return NULL;
	}else
	{
		fseek( theFile, 0, SEEK_END );
		fileLength = ftell( theFile );
	}
#endif

	printf("file length is %d for file %s\n", fileLength, fileName );
	rewind( theFile );
	fileData = (char*) malloc( sizeof(char) * fileLength );
	fread( fileData, fileLength, 1, theFile );
	fclose( theFile );
	
	// data check 1
	int	leftCount = 0, rightCount = 0;
	for( int i=0; i<fileLength; i++ )
	{
		if ( fileData[i] == '<' )
			leftCount++;
		if ( fileData[i] == '>' )
			rightCount++;
	}
	
	if ( leftCount != rightCount )
	{
		printf("ERROR: poXMLparse left-right bracket mismatch\n");
		// free(fileData);
		//return NULL;
	}

	
	// data check 2
	leftCount = rightCount = 0;
	for( int i=0; i<fileLength-1; i++ )
	{
		if ( fileData[i] == '<' && fileData[i+1] == '/' )
			rightCount++;
		else if ( fileData[i] == '<' && fileData[i+1] != '?' && fileData[i+1] != '!'  )
			leftCount++;
	}
	
	if ( leftCount == rightCount )
		skipFirstTag = false;
	else if ( leftCount == rightCount+1 )
		skipFirstTag = true;
	else
	{
		printf("ERROR: poXMLparse left-right tag mismatch (%d to %d)\n", leftCount, rightCount );
		free(fileData);
		return NULL;
	}
	
	// make tree
	int			stackHeight = 0;
	int			maxStackHeight = 1024;
	poXMLnode**	nodeStack = new poXMLnode* [maxStackHeight];

	int			tagStrStart = 0;
	//int			tagStrEnd = 0;
	int			dataStrStart = 0;
	int			parseState = 0;			// 0 in data, 1 in tag-open, 2 in tag-close
	
	int			maxDataLength = 8192;
	char*		tmpBuf = (char*) malloc( sizeof(char)*maxDataLength );
	
	bool		insideComment = false;
	
	// do actual parsing
	for( int i=0; i<fileLength-1; i++ )
	{

		if ( fileData[i] == '<' && fileData[i+1] == '!')
		{
			insideComment = true;
			continue;
		}
			
		if ( fileData[i] == '<' && fileData[i+1] == '?')
		{
			// optional data, skip it
			continue;
		}
		
		if ( fileData[i] == '?' && fileData[i+1] == '>')
		{
			// optional data, skip it
			continue;
		}
		
		if ( fileData[i] == '>' && fileData[i-1] == '/' )
		{
			printf("ending compact node\n");
						// pop stack
			if ( stackHeight-1 >= 0 )
				stackHeight--;
			else
			{
				printf("ERROR: poXMLparser: node stack under-run at %d of %d\n", i, fileLength );
				return NULL;
			}
			
			tagStrStart = i+2;
			parseState = 2;
			continue;
		}
		
		// node open, tag open
		if ( fileData[i] == '<' && fileData[i+1] != '/' && fileData[i+1] != '!')
		{
			if ( stackHeight+1 >= maxStackHeight )
			{
				printf("ERROR: poXMLparser: Stack overflow. Could not complete parsing.\n");
	
				for(int i=0; i<stackHeight; i++)
					delete nodeStack[i];
				delete [] nodeStack;
				free(fileData);
				free(tmpBuf);
				return NULL;
			}
		
			// make new node
			nodeStack[stackHeight] = new poXMLnode();
			
			// push stack
			if ( stackHeight-1 >= 0 )
				nodeStack[stackHeight-1]->addChild( nodeStack[stackHeight] );
				
			tagStrStart = i+1;
			parseState = 1;
			stackHeight++;
		}
		
		// tag close
		if ( fileData[i] == '>' )
		{
			if ( insideComment )
			{
				insideComment = false;
				continue;
			}
			
			// capture tag name string (for both open and close)
			if ( i-tagStrStart < maxDataLength )
			{
				strncpy( tmpBuf, &fileData[tagStrStart], i-tagStrStart );
				tmpBuf[ i-tagStrStart ] = 0;
				if ( parseState == 1 )		// end of open tag	
				{
					parseNameAndAttributes( nodeStack[stackHeight-1], tmpBuf );
					dataStrStart = i+1;
				}
				else if ( parseState == 2 )		// end of close tag
				{
					if ( nodeStack[stackHeight-1+1]->name != NULL &&
						 strcmp( nodeStack[stackHeight-1+1]->name, tmpBuf) != 0 )		// check name against one node higher
					{
						printf("ERROR: node name mismatch: #%s# and #%s#. Stopping parsing.\n", nodeStack[stackHeight-1+1]->name, tmpBuf );
						return NULL;
						
						// clean up please
						for(int i=0; i<stackHeight; i++)
							delete nodeStack[i];
						delete [] nodeStack;
						free(fileData);
						free(tmpBuf);
						return NULL;
					}
					//printf("close tag: #%s#\n", tmpBuf );
				}
			}
			else
				printf("ERROR: poXMLparser: node name too long at %d\n", i );
		}
		
		// node close
		if ( fileData[i] == '<' && fileData[i+1] == '/')
		{
			// capture data string (for nodes without children)
			if ( nodeStack[stackHeight-1]->numChildren == 0 )
			{
				if ( i-dataStrStart >= maxDataLength )
					printf("ERROR: poXMLparser: node data too long at %d\n", i );
				else
				{
					strncpy( tmpBuf, &fileData[dataStrStart], i-dataStrStart );
					tmpBuf[ i-dataStrStart ] = 0;
					nodeStack[stackHeight-1]->setData( tmpBuf );
				}
			}
			
			// pop stack
			if ( stackHeight-1 >= 0 )
				stackHeight--;
			else
			{
				printf("ERROR: poXMLparser: node stack under-run at %d of %d\n", i, fileLength );
				break;
			}
			
			tagStrStart = i+2;
			parseState = 2;
		}
		
		// printf("%c", fileData[i] );
	}
	
	
	free(fileData);
	free(tmpBuf);
	
	poXMLnode* rootNode = nodeStack[0];
	if ( skipFirstTag && rootNode->numChildren==1 )
		rootNode = rootNode->childList[0];
	return rootNode;
}

void		poXMLparser::parseNameAndAttributes( poXMLnode* N, char* nodeName )
{
	if ( nodeName == NULL )
		return;
	int L = (int)strlen( nodeName );
	
	char* attString = NULL;

	// set just name
	for( int i=0; i<L; i++ )
	{
		if ( nodeName[i]==' ' )
		{
			nodeName[i] = 0;
			N->setName( nodeName );
			nodeName[i] = ' ';
			attString = &nodeName[i+1];
			break;
		}
	}

	// if no attributes (got to end of string) just set name and return
	if ( attString==NULL )
	{
		N->setName( nodeName );
		return;
	}
	
	
	// if no attributes return
	if ( attString == NULL )
		return;
	
	// get length of attribute string
	L = strlen( attString );
		
	int nameStart = 0;
	int nameEnd = 0;
	int valueStart = 0;
	int valueEnd = 0;
	
	char	nameStr[512];
	char	valueStr[512];
	

	int	parseState = 0;
	for( int i=0; i<L; i++ )
	{
		// recording name string, looking for equals, copy name when found
		if ( parseState==0 )
		{			
			if ( attString[i]=='=' )
			{
				nameEnd = i;
				strncpy( nameStr, &attString[nameStart], nameEnd-nameStart );
				nameStr[ nameEnd-nameStart ] = 0;
				parseState = 1;
				continue;
			}
		}
		
		// found first quote, begin recording value
		if ( parseState==1 )
		{
			if ( attString[i]=='"' || attString[i]=='\'' )
			{
				parseState = 2;
				valueStart = i+1;
				continue;
			}
		}
		
		// recording value, looking for second quote, copy value when found
		if ( parseState==2 )
		{
			if ( attString[i]=='"' || attString[i]=='\'' )
			{
				parseState = 3; 
				valueEnd = i;
				
				strncpy( valueStr, &attString[valueStart], valueEnd-valueStart );
				valueStr[ valueEnd-valueStart ] = 0;
				N->addAttribute( nameStr, valueStr );
				continue;
			}
		}
		
		// between attributes, skipping spaces
		if ( parseState==3 )
		{
			if ( attString[i] != ' ' )
			{
				parseState = 0;
				nameStart = i;
				continue;
			}
		}
	}

}
