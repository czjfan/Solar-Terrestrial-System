struct VS_INPUT
{
	float4 Position : POSITION;
	float4 normal:NORMAL;
	float2 tex0:TEXCOORD;
};
struct VS_OUTPUT
{
	float4 Position:POSITION;
		float2 tex0:TEXCOORD0;
	float4 colorA:TEXCOORD1;
	float4 colorD:TEXCOORD2;
	float4 colorS:TEXCOORD3;
};
VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT Output;
	Output.Position = mul(input.Position, matWorldViewProjection);
		Output.tex0 = input.tex0;
		//��     ���ﶨ����VS������ṹ����Ҫ˵�����ǡ�POSITION����һ��semantics(����)��
		//	��������þ��Ǳ�ʶ�Ĵ��������д����ʾ��VS_ INPUT��Position��Ա����POSI TION�Ĵ����л�á����Ĵ����е���ֵ�Ѿ���Shader����֮ǰ��D3D׼�����ˡ���Ӧ��VS_ OUTPUT�е��������ʾShaderִ�����֮�����������ŵ��ĸ��Ĵ����С����������������֮��Ľ׶����ڶ���֮��ִ�����Բ�ֵ��Ȼ���ֵ�������PSʹ�á�
		//��     ������VS�����̵�������������������������͡�
		//��     ������任��ͶӰ�ռ䡣3DͶӰ��ʮ�ָ��ӵĹ��̣����˵���D3D�Ѿ�������������ĺܺã�ֻҪ���д���Ϳ���������ȷ�ľ��󣬲�������á������matWorldViewProjection����Ӧ�ó�������Ⱦ���̿�ʼ֮ǰͨ��ID3DXEFFECT�ӿ����õġ����ǰ��������硢�۲졢ͶӰ�任�ľ���
		//��     ֱ�ӽ���ͼ���귵�ء�
		//��     �������еĹ������㶼��������ռ���еģ���Ĭ������·�������ģ�Ϳռ��еģ�������Ҫ�����߱任������ռ��С����н���������λ������0����_41, _42, _43�ֱ��ʾ����ĵ�4�е�1��2��3�У������Ǳ���λ�����ĵط���������Ϊ���������������Ƕ��㣬����������λ�Ʊ任�Ĵ���ġ�
		//��     ����ִ���˷��ߺ͹��ߵĵ�ˡ�Normalize��������񻯣�dotִ�е�ˡ����Ƕ���HLSL�����ú�����
		//��     ������ߵķ���������ʹ�ù�ʽ��(�ο�[7]��P 53 Programing VertexShaders : RacorX3)
		//R = 2 * (N.L)*N - L��ʽ�У�N������������; L������������; R��������������

		Output.colorA = ka*lightAmbient*MaterialAmbient;
		Output.colorD = kd*lightdiffuse*(max(0, -DotNLInWorld))*MaterialDiffuse;
		float DotRV = dot(normalize(posinw - vViewPosition), Vreflect);
		Output.colorS = ks*lightspecular*pow(max(0, DotRV), ns)*max(0, -DotNLInWorld / abs(DotNLInWorld))*MaterialSpecular; ��
		return(Output);
}

struct PS_INPUT
{
	float2 tex0:TEXCOORD0;
	float4 colorA:TEXCOORD1;
	float4 colorD:TEXCOORD2;
	float4 colorS:TEXCOORD3;
};
float4 ps_main(PS_INPUT input) : COLOR0
{
	float4 finalcolor;
float4 diffusecolor = tex2D(DiffuseSampler,input.tex0);
finalcolor = ((input.colorA + input.colorD)*diffusecolor + input.colorS)*lightpower;
return finalcolor;
}
