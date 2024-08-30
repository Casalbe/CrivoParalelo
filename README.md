# Algoritmo de Crivo de Eratóstenes: Implementações Sequencial e Paralela

## 1. **Crivo de Eratóstenes**
O Crivo de Eratóstenes é um algoritmo clássico para encontrar todos os números primos até um dado número \( n \). A ideia básica é marcar iterativamente os múltiplos de cada número primo a partir de 2.

**Passos:**
1. **Inicialização:**
   - Crie um array `prime` de tamanho \( n+1 \) e inicialize todos os elementos como `1` (indicando que todos os números são inicialmente assumidos como primos).
   - Defina `prime[0]` e `prime[1]` como `0`, pois 0 e 1 não são números primos.

2. **Processo do Crivo:**
   - Começando pelo primeiro número primo (2), marque todos os seus múltiplos como não primos (defina como `0`).
   - Passe para o próximo número e repita o processo. O próximo número não marcado é o próximo primo.
   - Isso continua até a raiz quadrada de \( n \), pois qualquer número não primo maior que \( \sqrt{n} \) já teria sido marcado por um de seus fatores.

3. **Resultado:**
   - Após completar o crivo, o array `prime` terá `1` nas posições correspondentes aos números primos e `0` nas outras posições.

## 2. **Implementação Sequencial**
A versão sequencial do algoritmo segue diretamente os passos do Crivo de Eratóstenes. Ela itera pelo array e marca os múltiplos de cada número primo sem nenhum processamento paralelo.

- **Complexidade de Tempo:** \( O(n \log(\log(n))) \)
- **Complexidade de Espaço:** \( O(n) \)
