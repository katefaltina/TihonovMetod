<p>В данной задаче входной сигнал представляет собой сумму трёх гауссовых куполов и определяется по следующей формуле:</p>
<img src="https://github.com/katefaltina/TihonovMetod/assets/125446752/33e92166-6879-47e9-9416-bb14b816cfd2"> (1)
<p>где A_n – амплитуда n-го купола, μ_n – математическое ожидание, σ_n – среднеквадратичное отклонение.</p>
<p>Импульсная характеристика представляет собой гауссов купол, симметрично расположенный на концах импульсной последовательности. Формула вычисления имеет вид:</p>
<img src="https://github.com/katefaltina/TihonovMetod/assets/125446752/f2fa974d-1d1f-4bc2-88db-1734d57a64b8"> (2)
амплитуда выбрана единичная.
<p>В качестве заключительного шага подготовки к основной задаче необходимо с помощью циклической свёртки вычислить выходной сигнал линейной системы с полученной импульсной характеристикой. Для этого следует воспользоваться формулой:</p>
<img src="https://github.com/katefaltina/TihonovMetod/assets/125446752/81224316-d80c-425c-883d-d852e0a9a1bf"> (3)
<p>Теперь к выходному сигналу необходимо добавить шум и осуществить переход из временной области в частотную с помощью быстрого преобразования Фурье (БПФ). </p>
<p>Свёртка обладает таким свойством, что в частотной области переходит по теореме свёртки в простое умножение спектров сигналов:</p>
<img src="https://github.com/katefaltina/TihonovMetod/assets/125446752/d59644e2-067b-420f-ab28-37062e31660c"> (4)
<p>Для обращения свёртки  нужно решить уравнение в частотной области относительно искомого входного сигнала:</p>
<img src="https://github.com/katefaltina/TihonovMetod/assets/125446752/33e92166-6879-47e9-9416-bb14b816cfd2"> (5)
<ul>Однако такое решение является неустойчивым, так как H(ω) может быть очень малым (или вообще нулевым), поэтому предлагается ввести некоторую функцию R(α,ω). Такая функция является регуляризирующим оператором, если:
	<li>R(α,ω) определена в области   -∞<ω<∞;  α≥0</li>
	<li>для всех значений α≥0 и  ω  0≤R(α,ω)≤1</li>
	<li>для всякого α>0   R(α,ω) – чётная функция по ω, принадлежащая L_2 (-∞,∞)</li>
	<li>для всякого α>0   R(α,ω)→0 при |ω|→∞</li>
	<li>при  α→0   R(α,ω)→1, не убывая</li>
	<li>для всякого α>0   R(α,ω)/H(ω)∈L_2 (-∞,∞)</li>
	<li>для всякого ω≠0  R(α,ω)→0 при  α→∞.</li></ul>
<p>Тогда решение (5) перепишется следующим образом:</p>
<img src="https://github.com/katefaltina/TihonovMetod/assets/125446752/8b17ab58-67d1-4c3f-b24a-78ad78c0aa34"> (6)
<ul>где α – параметр регуляризации, а Q(ω) – заданная неотрицательная чётная функция, кусочно-непрерывная на любом конечном отрезке оси частот, причём 
	<li>Q(ω)>0 при ω≠0,Q(0)≥0</li>
	<li>при достаточно больших |ω|,Q(ω)≥C>0.</li></ul>
<p>Функцию Q(ω) можно выбрать в виде:</p>
<img src="https://github.com/katefaltina/TihonovMetod/assets/125446752/79c1845c-4ace-46e7-ab37-25eb4a17f0c8"> (7)
<p>где r – произвольное положительное число. В задаче будем полагать, что  r = 1.</p>
<p>После выбора Q(ω) значение параметра регуляризации α можно находить по невязке. Если уклонение правой части оценивать в метрике L2,  то квадрат расстояния между функциями g и  g_α (невязка) будет вычисляться по формуле</p>
<img src="https://github.com/katefaltina/TihonovMetod/assets/125446752/7dae5f9e-bf8f-4bc4-a73f-da6c76434d34"> (8)
<p>Φ(α) – невязка – это монотонно возрастающая функция переменного α, изменяющаяся от 0 до ‖g(t)‖^2. Следовательно, можно находить число α из условия  ρ_G (Au_α,g)=γ, где γ – погрешность.</p>

<ol>Алгоритм исследования:
<li>Сгенерировать входной сигнал и импульсную характеристику.</li>
<li>Вычислить с помощью циклической свёртки выходной сигнал и наложить белый гауссов шум.</li>
<li>Перейти из временной области в частотную.</li>
<li>С помощью алгоритма обращения свёртки методом регуляризации Тихонова восстановить входной сигнал.</li>
<li>Вывести график зависимости невязки (отклонения между входным и восстановленным сигналами) от уровня шума.</li></ol>
В ходе реализации программы исследование показало, что при увеличении уровня шума ухудшается восстановление  входного сигнала.
<img src="https://github.com/katefaltina/TihonovMetod/assets/125446752/77dcc6d4-a062-45bc-b9f1-977f2cfa9624">
