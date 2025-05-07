// URL base de la API (ESP8266 como Access Point)
const API_BASE_URL = "http://192.168.4.1";

// Función pura para obtener elementos del DOM
const getDomElements = () => ({
  alumnosTable: document.getElementById('alumnos-table')?.getElementsByTagName('tbody')[0],
  alumnoForm: document.getElementById('alumno-form'),
  btnRefresh: document.getElementById('btn-refresh'),
  btnCancel: document.getElementById('btn-cancel'),
  statusMessage: document.getElementById('status-message'),
  labInfoContent: document.getElementById('lab-info-content'),
  btnSearchId: document.getElementById('btn-search-id'),
  btnSearchName: document.getElementById('btn-search-name'),
  btnClearSearch: document.getElementById('btn-clear-search'),
  searchIdInput: document.getElementById('search-id'),
  searchNameInput: document.getElementById('search-name'),
  alumnoIdInput: document.getElementById('alumno-id'),
  nombreInput: document.getElementById('nombre'),
  direccionInput: document.getElementById('direccion'),
  correoInput: document.getElementById('correo')
});

// Estado de la aplicación (closure)
const createAppState = () => {
  let allAlumnos = [];
  let currentSearchTerm = '';

  return {
    getAllAlumnos: () => [...allAlumnos],
    setAllAlumnos: (alumnos) => { allAlumnos = [...alumnos]; },
    getCurrentSearchTerm: () => currentSearchTerm,
    setCurrentSearchTerm: (term) => { currentSearchTerm = term; },
    resetSearchTerm: () => { currentSearchTerm = ''; }
  };
};

// Funciones de utilidad
const showMessage = (message, type) => {
  const { statusMessage } = getDomElements();
  if (statusMessage) {
    statusMessage.textContent = message;
    statusMessage.className = `status ${type}`;
    statusMessage.style.display = 'block';
    
    setTimeout(() => {
      statusMessage.style.display = 'none';
    }, 5000);
  }
};

// Función para manejar fetch con estilo funcional
const fetchData = async (url, options = {}) => {
  try {
    const response = await fetch(url, options);
    return response.ok 
      ? Promise.resolve(await response.json())
      : Promise.reject(new Error(`Error ${response.status}: ${response.statusText}`));
  } catch (error) {
    showMessage(error.message, 'error');
    return Promise.reject(error);
  }
};

// Funciones para manejar datos
const loadAlumnos = async (state) => {
  try {
    const alumnos = await fetchData(`${API_BASE_URL}/api/alumnos`);
    state.setAllAlumnos(alumnos);
    renderAlumnos(alumnos, state.getCurrentSearchTerm());
    showMessage('Lista de alumnos actualizada', 'success');
  } catch (error) {
    console.error('Error al cargar alumnos:', error);
  }
};

const loadLabInfo = async () => {
  try {
    const labInfo = await fetchData(`${API_BASE_URL}/api/laboratorio`);
    renderLabInfo(labInfo);
  } catch (error) {
    const { labInfoContent } = getDomElements();
    if (labInfoContent) {
      labInfoContent.innerHTML = `<p class="error">Error al cargar información del laboratorio</p>`;
    }
  }
};

// Funciones de renderizado
const renderLabInfo = (labInfo) => {
  const { labInfoContent } = getDomElements();
  if (labInfoContent) {
    labInfoContent.innerHTML = `
      <h3>${labInfo.nombre}</h3>
      <p><strong>Universidad:</strong> ${labInfo.universidad}</p>
      <p><strong>Departamento:</strong> ${labInfo.departamento}</p>
      <p><strong>Responsable:</strong> ${labInfo.profesor}</p>
      <p><strong>Ubicación:</strong> ${labInfo.ubicacion}</p>
      <p><strong>Descripción:</strong> ${labInfo.descripcion}</p>
      <p><strong>Equipamiento:</strong> ${labInfo.equipamiento}</p>
    `;
  }
};

const highlightMatch = (text, term) => 
  term 
    ? text.replace(new RegExp(term, 'gi'), match => `<span class="highlight-name">${match}</span>`)
    : text;

const renderAlumnos = (alumnos, searchTerm = '') => {
  const { alumnosTable } = getDomElements();
  if (!alumnosTable) return;

  alumnosTable.innerHTML = '';
  
  alumnos.forEach(alumno => {
    const row = document.createElement('tr');
    const shouldHighlight = searchTerm && 
      (alumno.id.toString() === searchTerm || 
       alumno.nombre.toLowerCase().includes(searchTerm.toLowerCase()));
    
    if (shouldHighlight) {
      row.classList.add('highlight');
    }
    
    row.innerHTML = `
      <td>${alumno.id}</td>
      <td>${highlightMatch(alumno.nombre, searchTerm)}</td>
      <td>${alumno.direccion}</td>
      <td>${alumno.correo}</td>
      <td>
        <button onclick="handleEditAlumno(${alumno.id})">Editar</button>
        <button onclick="handleDeleteAlumno(${alumno.id})" class="btn-danger">Eliminar</button>
      </td>
    `;
    
    alumnosTable.appendChild(row);
  });
};

// Funciones de búsqueda
const searchById = (state) => {
  const { searchIdInput } = getDomElements();
  const id = parseInt(searchIdInput?.value || '');

  return isNaN(id)
    ? showMessage('Ingrese un ID válido', 'error')
    : state.getAllAlumnos().find(alumno => alumno.id === id)
      ? (() => {
          state.setCurrentSearchTerm(id.toString());
          renderAlumnos([state.getAllAlumnos().find(alumno => alumno.id === id)], id.toString());
          showMessage(`Alumno con ID ${id} encontrado`, 'success');
        })()
      : showMessage(`No se encontró alumno con ID ${id}`, 'error');
};

const searchByName = (state) => {
  const { searchNameInput } = getDomElements();
  const name = searchNameInput?.value.trim() || '';

  return !name
    ? showMessage('Ingrese un nombre para buscar', 'error')
    : (() => {
        const term = name.toLowerCase();
        const alumnosEncontrados = state.getAllAlumnos().filter(alumno => 
          alumno.nombre.toLowerCase().includes(term)
        );
        
        alumnosEncontrados.length > 0
          ? (() => {
              state.setCurrentSearchTerm(term);
              renderAlumnos(alumnosEncontrados, term);
              showMessage(`Encontrados ${alumnosEncontrados.length} alumnos`, 'success');
            })()
          : showMessage('No se encontraron alumnos con ese nombre', 'error');
      })();
};

const clearSearch = (state) => {
  const { searchIdInput, searchNameInput } = getDomElements();
  if (searchIdInput) searchIdInput.value = '';
  if (searchNameInput) searchNameInput.value = '';
  state.resetSearchTerm();
  renderAlumnos(state.getAllAlumnos());
  showMessage('Búsqueda limpiada', 'success');
};

// Funciones para manejar el formulario
const handleFormSubmit = (state) => async (e) => {
  e.preventDefault();
  
  const { alumnoIdInput, nombreInput, direccionInput, correoInput } = getDomElements();
  const alumno = {
    nombre: nombreInput?.value || '',
    direccion: direccionInput?.value || '',
    correo: correoInput?.value || ''
  };
  
  try {
    const url = alumnoIdInput?.value 
      ? `${API_BASE_URL}/api/alumnos/${alumnoIdInput.value}`
      : `${API_BASE_URL}/api/alumnos`;
    
    const method = alumnoIdInput?.value ? 'PUT' : 'POST';
    
    await fetchData(url, {
      method,
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify(alumno)
    });
    
    resetForm();
    await loadAlumnos(state);
    showMessage(`Alumno ${alumnoIdInput?.value ? 'actualizado' : 'agregado'} correctamente`, 'success');
  } catch (error) {
    console.error('Error al guardar alumno:', error);
  }
};

const resetForm = () => {
  const { alumnoForm, alumnoIdInput, btnCancel } = getDomElements();
  if (alumnoForm) alumnoForm.reset();
  if (alumnoIdInput) alumnoIdInput.value = '';
  if (btnCancel) btnCancel.style.display = 'none';
};

const cancelEdit = () => {
  resetForm();
  showMessage('Edición cancelada', 'success');
};

// Funciones globales para los botones de la tabla
window.handleEditAlumno = async (id) => {
  try {
    const alumno = await fetchData(`${API_BASE_URL}/api/alumnos/${id}`);
    const { alumnoIdInput, nombreInput, direccionInput, correoInput, btnCancel } = getDomElements();
    
    if (alumnoIdInput) alumnoIdInput.value = alumno.id;
    if (nombreInput) nombreInput.value = alumno.nombre;
    if (direccionInput) direccionInput.value = alumno.direccion;
    if (correoInput) correoInput.value = alumno.correo;
    if (btnCancel) btnCancel.style.display = 'inline-block';
    
    const form = document.getElementById('alumno-form');
    if (form) form.scrollIntoView({ behavior: 'smooth' });
    
    showMessage(`Editando alumno: ${alumno.nombre}`, 'success');
  } catch (error) {
    console.error('Error al editar alumno:', error);
  }
};

window.handleDeleteAlumno = async (id) => {
  if (!confirm('¿Estás seguro de que deseas eliminar este alumno?')) return;
  
  try {
    await fetchData(`${API_BASE_URL}/api/alumnos/${id}`, {
      method: 'DELETE'
    });
    
    const state = appState;
    await loadAlumnos(state);
    showMessage('Alumno eliminado correctamente', 'success');
  } catch (error) {
    console.error('Error al eliminar alumno:', error);
  }
};

// Inicialización de la aplicación
const initializeApp = () => {
  const state = createAppState();
  const elements = getDomElements();

  // Cargar datos iniciales
  document.addEventListener('DOMContentLoaded', () => {
    loadAlumnos(state);
    loadLabInfo();
  });

  // Configurar eventos
  elements.btnRefresh?.addEventListener('click', () => loadAlumnos(state));
  elements.btnCancel?.addEventListener('click', cancelEdit);
  elements.alumnoForm?.addEventListener('submit', handleFormSubmit(state));
  elements.btnSearchId?.addEventListener('click', () => searchById(state));
  elements.btnSearchName?.addEventListener('click', () => searchByName(state));
  elements.btnClearSearch?.addEventListener('click', () => clearSearch(state));
  elements.searchNameInput?.addEventListener('input', (e) => 
    e.target.value === '' && clearSearch(state)
  );

  return state;
};

// Iniciar la aplicación
const appState = initializeApp();