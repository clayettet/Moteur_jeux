#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;

attribute vec4 a_position;
attribute vec2 a_texcoord;

varying vec2 v_texcoord;
varying vec4 my_color;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;
    if (a_position.y > 0.4)
    {
        my_color = vec4(0.3, 0.3, 0.3, 1.0);
    }
    else
    {
        my_color = vec4(0.3, 0.8, 0.3, 1.0);
    }

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
}
//! [0]
