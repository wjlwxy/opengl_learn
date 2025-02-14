#shader Vertex
// ����ʹ��GLSL�İ汾Ϊ3.3���Ĺ淶��#versionָ�������shader����ĵ�һ�У�����ָ��ʹ�õ�GLSL�汾��
#version 330 core

layout(location = 0) in vec4 position; // �����������position������Ϊvec4����ά��������layout(location = 0)ָ����������������λ������Ϊ0����Ӧ�ó����У�����ͨ��������������ݴӶ��㻺�������ݸ�������ɫ����
// texCoordͨ��VertexArray::AddBuffer�󶨶������飬�Ӷ��� �������� ���� texCoord
layout(location = 1) in vec2 texCoord; // �����������texCoord������Ϊvec2����ά��������layout(location = 1)ָ����������������λ������Ϊ1���������ͨ�����ڴ洢�������꣬�Ա���Ƭ����ɫ���ж�������в�����

out vec2 v_TexCoord; // �����������v_TexCoord������Ϊvec2��out �ؼ��ֱ�ʾ���������ֵ���Ӷ�����ɫ�����ݵ�Ƭ����ɫ�������ﴫ�ݵ����������꣬�Ա�Ƭ����ɫ������ʹ����Щ������ȷ��������ͼ���в����ĸ�λ�õ���ɫ��

uniform mat4 u_MVP; // uniform ��ʾ�����ݴ�cpu������

void main()
{
	gl_Position = u_MVP * position; // �������λ����Ϣ���������position���ݸ�OpenGL�ڲ���gl_Position������gl_Position��һ��������������������ָ�������ڲü��ռ��е�λ�á�
	v_TexCoord = texCoord; // ����������������texCoord���ݵ��������v_TexCoord���Ա���Ƭ����ɫ����ʹ�á�
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color; // ����һ���������color������vec4���͵ģ�������ɫ����ɫ����ɫ��͸���ȷ�������layout(location = 0)ָ��������������λ������Ϊ0����������ں��������н����ڰ�Ƭ����ɫ�������ӳ�䵽֡��������ĳ����ɫ��������

in vec2 v_TexCoord;

uniform vec4 u_Color; // ����һ��uniform����u_Color������vec4���͵ġ�uniform�������ڶ����ɫ��֮�乲��ģ�����ͨ�����ڴ���ȫ�����ݣ��������Ϣ��������ɫ�ȣ���u_Color��������ܱ����ڿ���Ƭ�ε���ɫ��
uniform sampler2D u_Texture; // ����һ��uniform����u_Texture������sampler2D���͵ġ�sampler2D������Ƭ����ɫ���з��ʶ�ά��������u_Texture���������������в�����ɫ������˲���ʱֻ�贫��λ���ĸ�slot��

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord); // ʹ��texture������u_Texture�����в�����ɫ��v_TexCoord�����Զ�����ɫ�����������꣬��ָʾ�˴������в����ĸ�λ�õ���ɫ�������õ�����ɫ���洢�ڱ���texColor�С�
	color = texColor; // 
};