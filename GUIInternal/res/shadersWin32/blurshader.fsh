uniform sampler2D u_diffuse_texture;
varying vec2 v_uvcoord0;

/*
void main()
{
	gl_FragColor = texture2D(u_diffuse_texture, v_uvcoord0);
}
*/

/*
const float blurSize = 1.0/800.0;
const float weightSum = (70.0 + 2.0 * (1.0 + 8.0 + 28.0 + 56.0));

void main()
{
    highp vec4 sum = vec4(0.0, 0.0, 0.0, 1.0);
    
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x - 4.0*blurSize, v_uvcoord0.y)) * 1.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x - 3.0*blurSize, v_uvcoord0.y)) * 8.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x - 2.0*blurSize, v_uvcoord0.y)) * 28.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x - blurSize, v_uvcoord0.y)) * 56.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x, v_uvcoord0.y)) * 70.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x + blurSize, v_uvcoord0.y)) * 56.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x + 2.0*blurSize, v_uvcoord0.y)) * 28.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x + 3.0*blurSize, v_uvcoord0.y)) * 8.0 / weightSum;
    sum += texture2D(u_diffuse_texture, vec2(v_uvcoord0.x + 4.0*blurSize, v_uvcoord0.y)) * 1.0 / weightSum;
    
    gl_FragColor = sum;
}
*/


void main()
{
    vec4 sum = vec4(0);
    vec4 clr = vec4(0);
    int j=0;
    int i=0;
    
    //
    i=-2;    j=-3;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    
    i=-2;    j=-2;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    
    i=-2;    j=-1;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    
    i=-2;    j=0;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    
    i=-2;    j=1;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    
    i=-2;    j=2;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    //

    //
    i=-1;    j=-3;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    
    i=-1;    j=-2;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    
    i=-1;    j=-1;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    
    i=-1;    j=-1;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004)*1.2;
    
    i=-1;    j=0;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004)*1.2;
    
    i=-1;    j=1;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004)*1.2;
    //
    
    //
    i=0;    j=-3;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.04);
    
    i=0;    j=-2;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.04);
    
    i=0;    j=-1;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.04);
    
    i=0;    j=-1;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004)*1.2;
    
    i=0;    j=0;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004)*1.2;
    
    i=0;    j=1;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004)*1.2;
    //
    
    //
    i=1;    j=-3;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    
    i=1;    j=-2;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    
    i=1;    j=-1;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    
    i=1;    j=0;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    
    i=1;    j=1;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    
    i=1;    j=2;
    sum += texture2D(u_diffuse_texture, v_uvcoord0 + vec2(j, i)*0.004) * 0.25;
    //

    
    if (texture2D(u_diffuse_texture, v_uvcoord0).r < 0.3)
    {
        clr = sum*sum*0.012 + texture2D(u_diffuse_texture, v_uvcoord0);
    }
    else
    {
        if (texture2D(u_diffuse_texture, v_uvcoord0).r < 0.5)
        {
            clr = sum*sum*0.009 + texture2D(u_diffuse_texture, v_uvcoord0);
        }
        else
        {
            clr = sum*sum*0.0075 + texture2D(u_diffuse_texture, v_uvcoord0);
        }
    }
    
    gl_FragColor = clr;
}

