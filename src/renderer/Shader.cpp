
#include "renderer/Shader.hpp"
#include "renderer/Texture.hpp"
#include "utils/CheckGLError.hpp"

#include <iostream>
//#include <stdlib.h>
//#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "glm/glm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "kiwi/core/Node.hpp"
#include "kiwi/core/InputPort.hpp"
#include "kiwi/core/OutputPort.hpp"

using namespace std;

namespace renderer{

static void validateProgram(GLuint program) {
    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;

    glGetProgramInfoLog(program, BUFFER_SIZE, &length, buffer);
    if (length > 0)
        cout << "Program " << program << " link error: " << buffer << endl;

    glValidateProgram(program);
    GLint status;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status == GL_FALSE)
		cout << "Error validating shader " << program << endl;
}


bool Shader::build(const string& vs_src,const string& fs_src, const LocationMap& locations)
{
    cout << "Shader::build" << endl;
    CHECKERROR
    const char* vs_text = vs_src.c_str();
    const char* fs_text = fs_src.c_str();

    glShaderSource(_vsId, 1, &vs_text, 0); 
    glCompileShader(_vsId);
    CHECKERROR
    glShaderSource(_fsId, 1, &fs_text, 0);
    glCompileShader(_fsId);

    CHECKERROR
    _id = glCreateProgram(); 
    glAttachShader(_id, _vsId);
    glAttachShader(_id, _fsId);

    _locations = locations;

    CHECKERROR
    glBindAttribLocation(_id, 0, "in_Position");
    glBindAttribLocation(_id, 1, "in_Color");

    CHECKERROR
    glLinkProgram(_id);
    
    for(auto it = _locations.begin(); it != _locations.end(); ++it)
    {
        if(it->second.type & UNIFORM)
        {
            it->second.location = glGetUniformLocation( _id, it->first.c_str() );
            cout << "location: " << it->first << endl;
            CHECKERROR
        }
    }

    CHECKERROR
    validateProgram(_id);

    _state |= VALID;
    cout << "shader state: "<<_state << endl;
    return true;
}

bool Shader::bind()
{
    /*
    if( !(_state & VALID) )
    {
        cout << "Shader::bind Error! invalid state "<< _state << endl;
        assert(false);
        return false;
    }*/
    glUseProgram(_id);
    _state &= BINDED;
    return true;
}

bool Shader::hasLocation(const std::string& name) const
{
    for(auto it = locations_begin(); it!= locations_end(); ++it )
    {
        if( name == it->first )
            return true;
    }
    return false;
}

const Shader::Location* Shader::location(const string& name)
{
    if( ! hasLocation(name) )
        return 0;
    return &_locations[name];
}

GLuint getGLTextureLocation( int i )
{
    switch( i )
    {   
        case 0 : return GL_TEXTURE0;
        case 1 : return GL_TEXTURE1;
        case 2 : return GL_TEXTURE2;
        case 3 : return GL_TEXTURE3;
        case 4 : return GL_TEXTURE4;
        case 5 : return GL_TEXTURE5;
        case 6 : return GL_TEXTURE6;
        case 7 : return GL_TEXTURE7;
    }
    assert( "bad"=="parameter" );
    return 0;
}

bool ShaderNodeUpdater::update( const kiwi::core::Node& node )
{
    //for( auto it = _shader->locations_begin(); it != _shader->locations_end(); ++it )
    for( int i = 0; i < node.inputs().size(); ++i )
    {
        auto name = node.inputName(i);
        auto loc = _shader->location( name );
        switch( loc->type & ~Shader::UNIFORM )
        {
            case Shader::INT:
            {
                _shader->uniform1i( name, *node.input(i).dataAs<int>() );
                break;
            }
            case Shader::FLOAT:
            {
                _shader->uniform1f( name, *node.input(i).dataAs<float>() );
                break;
            }
            case Shader::FLOAT2:
            {
                auto v2 = node.input(i).dataAs<glm::vec2>();
                _shader->uniform2f( name, v2->x, v2->y );
                break;
            }
            case Shader::FLOAT3: 
            {
                auto v3 = node.input(i).dataAs<glm::vec3>();
                _shader->uniform3f( name, v3->x, v3->y, v3->z );
                break;
            }
            case Shader::MAT4F:
            {
                _shader->uniformMatrix4fv( name
                        , &(*node.input(i).dataAs<glm::mat4>())[0][0] );
                break;
            }
            case Shader::TEXTURE2D:
            {
                _shader->uniform1i( name, node.input(i).dataAs<Texture2D>()->id() );
                break;
            }
        }
    }
    for( int i = 0; i < node.outputs().size(); ++i )
    {
        glActiveTexture( getGLTextureLocation(i) );
        auto tex = node.output(i).dataAs<Texture2D>();
        glBindTexture( GL_TEXTURE_2D, tex->id() );
    }
}


/*
const kiwi::core::NodeTypeInfo * RegisterShaderNode(const kiwi::string& name, Shader& shader)
{
    NodeLayoutDescriptor layout;
    for( auto it = shader.locations_begin(); it != shader.locations_end(); ++it )
    {
        if ( it->type & Shader::UNIFORM )
        {
            auto dataType = kiwi::core::DataTypeManager::TypeOf( ShaderType(it->second.type) );
            assert( dataType != 0 );
            layout.inputs.push_back( {it->first, dataType, READ} );
        }
        
    }
    KIWI_NOT_IMPLEMENTED("RegisterShaderNode");
    return 0;
}
*/
}//namespace
