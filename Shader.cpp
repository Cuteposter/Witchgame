/**
Copyright (c) 2012 - Luu Gia Thuy

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Shader.h"
#include <GL/glew.h>

CShader::CShader()
	: m_VertexShader(0),
	m_GeometricShader(0),
	m_FragmentShader(0),
	m_Program(0)
{
}

CShader::~CShader()
{
}

int CShader::GetUniformIndex(const char* inVarName)
{
	return GetVariableIndex(inVarName, true);;
}

int CShader::GetAttributeIndex(const char* inVarName)
{
	return GetVariableIndex(inVarName, false);
}

int CShader::GetVariableIndex(const char *inVarName, bool inIsUniform)
{
	int theResult = -1;

	if (m_Program != 0)
	{
		TVariableMap::iterator iter;
		iter = m_VariableMap.find(inVarName);
		if (iter == m_VariableMap.end())
		{
			if (inIsUniform) // uniform variables
				theResult = glGetUniformLocation(m_Program, inVarName);
			else // attribute variables
				theResult = glGetAttribLocation(m_Program, inVarName);
			if (theResult != -1)
				m_VariableMap[inVarName] = theResult;
		}
		else
			theResult = iter->second;
	}

	return theResult;
}